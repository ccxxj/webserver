#include "RequestHandler.hpp"

#include <sstream> // for converting int to string
#include <stdio.h> // for perror
#include <stdlib.h> //for atoi
#include <sys/event.h>//for kqueue
#include <unistd.h>

#include "Exceptions/RequestException.hpp"
#include "../Utility/Utility.hpp"
#include "../Constants.hpp"

namespace HTTP {
	RequestHandler::RequestHandler(RequestHandlerDelegate& delegate, Config::ConfigData *config_data, ListenInfo& listen_info)
	: _http_request_message()
	, _http_response_message()
	, _delegate(delegate)
	, _parser(&_http_request_message, &_http_response_message)
	, _config_data(config_data)
	, _connection_listen_info(listen_info)
	, response_handler(&_http_request_message, &_http_response_message)
	, _cgi_handler()
	, response_ready(false)
	{
	}

	RequestHandler::~RequestHandler(){}

	void RequestHandler::handle_http_request(int kq, int socket_fd) {
		char buf[4096];
		ssize_t bytes_read = _delegate.receive(buf, sizeof(buf));
		if (bytes_read == 0) {
			_delegate.close();
		} else if (bytes_read == Constants::ERROR) {
			perror("recv error");
			_delegate.close();
		} else {
			try {
				_parser.parse_HTTP_request(buf, bytes_read);
			}
			catch(const Exception::RequestException& e)
			{
				_handle_request_exception(e.get_error_status_code());
				Utility::logger("Request  [Bad Request]", YELLOW);
				response_handler.handle_error(e.get_error_status_code()); //error response is built, and will be sent below
				response_ready = true;
			}
			if (!_parser.is_parsing_finished()) {
				return;
			}
			if (!response_ready) { // checking if the response with the error code has been filled
				if(!_process_http_request(kq, socket_fd)) //this means the cgi is encounted and data prepared
				{
					//add writing event
					struct kevent kev;
					int write_fd = _cgi_handler.get_write_fd();
					EV_SET(&kev, write_fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
					if (kevent(kq, &kev, 1, NULL, 0, NULL)<0){
						fprintf(stderr,"kevent failed.");
						return;
					}
				}
				else
					response_ready = true;
			}
		}
	}

	void RequestHandler::send_response() {
		if (response_ready) {
			std::string& response = _http_response_message.get_complete_response();
			_delegate.send(response, response.size());
		}
	}

	void RequestHandler::_handle_request_exception(HTTPResponse::StatusCode code) {
		_http_response_message.set_status_code(_convert_status_code_to_string(static_cast<int>(code)));
		_http_response_message.set_reason_phrase(HTTPResponse::get_reason_phrase(code));
	}

	const std::string RequestHandler::_convert_status_code_to_string(const int code) {
		std::string stringified_code;
		std::stringstream sstream;
		sstream << code;
		sstream >> stringified_code;
		return stringified_code;
	}

	bool RequestHandler::RequestHandler::_process_http_request(int kq, int socket_fd) { //TODO Logger to say which server & block is matched with the port info
		const Config::ServerBlock *virtual_server = _find_virtual_server();
		const Config::LocationBlock *location = _match_most_specific_location(virtual_server);
		response_handler.set_config_rules(virtual_server, location);
		
		return response_handler.create_http_response(kq, _cgi_handler, socket_fd); //FROM here, it's moving to ResponseHandler
	}

	const Config::ServerBlock* RequestHandler::_find_virtual_server() {
		std::vector<const Config::ServerBlock*> matching_servers;
		for (std::vector<Config::ServerBlock>::const_iterator it = _config_data->get_servers().begin(); it != _config_data->get_servers().end(); it++) {
			for (std::set<std::string>::const_iterator port = it->get_listen().begin(); port != it->get_listen().end(); port++) {
				int server_port = std::atoi((*port).c_str());
				if (server_port == _connection_listen_info.port) {
					matching_servers.push_back(&(*it));
					break; // if a port is matched, no need to check other ports of same server block
				}
			}
		}
		if (matching_servers.size() == 1) // if only 1 server matches, that's it, return it
			return matching_servers.front();
		else if (matching_servers.size() == 0) // if none is matched, return default server
			return &_config_data->get_servers().front();
		else // if more than 1 server is matched, get host name from request header and match based on server names
			return _match_server_based_on_server_name(matching_servers);
	}

	const Config::ServerBlock* RequestHandler::_match_server_based_on_server_name(std::vector<const Config::ServerBlock*> matching_servers) {
		std::string host = "";
		if(_http_request_message.has_header_field("HOST"))
			host = _http_request_message.get_header_value("HOST"); 
		for (std::vector<const Config::ServerBlock*>::iterator it = matching_servers.begin(); it != matching_servers.end(); it++)
			for (std::vector<std::string>::const_iterator srv_name = (*it)->get_server_name().begin(); srv_name != (*it)->get_server_name().end(); srv_name++)
				if ((*srv_name).compare(host) == 0)
					return *it; // the first one is used even if there might be multiple matches
		return matching_servers.front(); //returns the default one for that ip + port if multiple servers match ip+port but no match with server_name,
	}

	const Config::LocationBlock* RequestHandler::_match_most_specific_location(const Config::ServerBlock *server) {
		std::vector<const Config::LocationBlock*> matched_locations;
		const std::vector<std::string> uri_paths = _http_request_message.get_uri().get_path();
		for (std::vector<Config::LocationBlock>::const_iterator it = server->get_location().begin(); it != server->get_location().end(); it++) {
			const std::string loc_route = it->get_route();
			std::string searched_uri = "";
			for (size_t i = 0; i < uri_paths.size(); i++) {
				searched_uri += uri_paths[i] + "/";
				if (loc_route.compare(searched_uri) == 0) {
					matched_locations.push_back(&(*it));
					break; // no need to look further
				}
			}
		}
		if (matched_locations.size() == 0) // if no match
			return NULL;
		else //find the longest route that matches
		{
			const Config::LocationBlock* match;
			size_t length = 0;
			for (std::vector<const Config::LocationBlock*>::iterator it = matched_locations.begin(); it != matched_locations.end(); it++) {
				if((*it)->get_route().length() > length)
				{
					length = (*it)->get_route().length();
					match = (*it);
				}
			}
			return match;
		}
	}
	
	HTTPResponse::ResponseMessage &RequestHandler::get_http_response_message(){
		return _http_response_message;
	}

	void RequestHandler::set_response_true(){
		response_ready = true;
	}
	
	void RequestHandler::set_cgi_handler(CGIHandler cgi_handler){
		_cgi_handler = cgi_handler;
	}

    // CGIHandler RequestHandler::get_cgi_handler(){
	// 	return _cgi_handler;
	// }

	void RequestHandler::execute_cgi(int kq){
		_cgi_handler.execute_cgi(kq);
	}

	void RequestHandler::handle_internal_server_error(){
		response_handler.handle_error(static_cast<HTTPResponse::StatusCode>(500));//500 is the code for internal server error
	}

	int RequestHandler::get_cgi_write_fd() const{
		return _cgi_handler.get_write_fd();
	}

	int RequestHandler::get_cgi_read_fd() const{
		return _cgi_handler.get_read_fd();
	}

	const std::string RequestHandler::get_request_message_body() const{
		return _http_request_message.get_message_body();
	}

	bool RequestHandler::get_search_cgi_extention_result() const{
		return _cgi_handler.get_search_cgi_extention_result();
	}
}

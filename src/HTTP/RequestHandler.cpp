#include "RequestHandler.hpp"

#include <sstream> // for converting int to string
#include <stdio.h> // for perror

#include "Exceptions/RequestException.hpp"
#include "../globals.hpp"

namespace HTTP {
	RequestHandler::RequestHandler(RequestHandlerDelegate& delegate, Config::ConfigData *config_data, ListenInfo& listen_info)
	: _http_request_message()
	, _http_response_message()
	, _delegate(delegate)
	, _parser(&_http_request_message, &_http_response_message)
	, _config_data(config_data)
	, _connection_listen_info(listen_info)
	{
	}

	RequestHandler::~RequestHandler(){}

	void RequestHandler::handle_http_request() {
		char buf[4096];
		ssize_t bytes_read = _delegate.receive(buf, sizeof(buf));
		if (bytes_read == 0) {
			_delegate.close();
		} else if (bytes_read == ERROR) {
			perror("recv error");
			_delegate.close();
		} else {
			std::cout << "\nRead " << bytes_read << " bytes\n";
			std::cout.write(buf, bytes_read);
			try {
				_parser.parse_HTTP_request(buf, bytes_read);
			}
			catch(const Exception::RequestException& e)
			{
				_handle_request_exception(e.get_error_status_code());
			}
			if (!_parser.is_parsing_finished()) {
				// std::cout << "parsing is finished?" << std::endl;
				//FIXME this prevent me from moving on to processing the request
				// return;
			}
			_process_http_request();
			std::string status_code = _http_response_message.get_status_code();
			std::string reason_phrase = _http_response_message.get_reason_phrase();
			std::string status_line = _http_response_message.get_HTTP_version() + " " + status_code + " " + reason_phrase + "\r\n\r\n";
			// std::cout << "\n\nRESPONSE:\nStatus line: " << status_line << std::endl;
			_delegate.send(&status_line[0], status_line.size()); //TODO: replace by full response
			_delegate.close();
		}
	}

	void RequestHandler::_handle_request_exception(HTTPResponse::StatusCode code) {
		_http_response_message.set_status_code(_convert_status_code_to_string(static_cast<int>(code)));
		_http_response_message.set_reason_phrase(HTTPResponse::get_reason_phrase(code));
		std::cout << "\n\nstatus code: " << _http_response_message.get_status_code() << std::endl;
		std::cout << "\n\nreason phrase: " << _http_response_message.get_reason_phrase() << std::endl;
	}

	const std::string RequestHandler::_convert_status_code_to_string(const int code) {
		std::string stringified_code;
		std::stringstream sstream;
		sstream << code;
		sstream >> stringified_code;
		return stringified_code;
	}

	void RequestHandler::RequestHandler::_process_http_request() {
		const Config::ServerBlock *virtual_server = _find_virtual_server();
		std::cout << "check ret of find virt serv " << virtual_server->get_client_max_body_size() << std::endl;
		const Config::LocationBlock *location = _match_most_specific_location(virtual_server);
		//FIXME if no loc is matched then there is no method limitation, adjust accordingly? or move to creating response and connect there ?
		if(!_verify_method(location->get_limit_except()))
			return _http_response_message.create_response(HTTPResponse::MethodNotAllowed);//FIXME method not allowed error added to the response and don't go further
		_http_response_message.create_response(); //FROM this point onward, it's moving to Response and from there to ResponseHandler
		// create_response(); // finalizing_request!
			// get_http_version();
			// get_response_headers();

	}

	const Config::ServerBlock* RequestHandler::_find_virtual_server() {
		std::cout << "...Finding Virtual Server for..." << "\033[31mip: " << _connection_listen_info.ip;
		std::cout << " port: " << _connection_listen_info.port << "\033[0m\n" << std::endl;
		// match server based on request ip + port
		std::vector<const Config::ServerBlock*> matching_servers;
		for (std::vector<Config::ServerBlock>::const_iterator it = _config_data->get_servers().begin(); it != _config_data->get_servers().end(); it++) {
			for (std::set<std::string>::const_iterator port = it->get_listen().begin(); port != it->get_listen().end(); port++) {
			//TODO Discuss how you are getting this information.
			//I tried to convert Connection.ip and addr with ntohs functions but I get a weird integer
				uint32_t server_port = std::atoi((*port).c_str());
				if (server_port == _connection_listen_info.port) {
					matching_servers.push_back(&(*it));
					break; // if one port is matched, there is no need to check other ports of same server block
				}
			}
		}
		std::cout << matching_servers.size() <<  " server block matched based on ip + port" << std::endl;
		if (matching_servers.size() == 1) // if only 1 server matches, that's it, return it
			return matching_servers.front();
		else if (matching_servers.size() == 0) // if none is matched, return default server
			return &_config_data->get_servers()[0];
		else // if more than 1 server is matched, get host name from request header and match based on server names
			return _match_one_based_on_server_name(matching_servers);
	}

	const Config::ServerBlock* RequestHandler::_match_one_based_on_server_name(std::vector<const Config::ServerBlock*> matching_servers) {
		std::string host = _http_request_message.get_header_value("Host"); //TODO what do we have as return value
		//if (host.empty()) //TODO no host header error?
		for (std::vector<Config::ServerBlock>::const_iterator it = _config_data->get_servers().begin(); it != _config_data->get_servers().end(); it++) {
			for (std::set<std::string>::const_iterator srv_name = it->get_server_name().begin(); srv_name != it->get_server_name().end(); srv_name++) {
				if ((*srv_name).compare(host) == 0)
					return *it;
			}
		}
	}

	const Config::LocationBlock* RequestHandler::_match_most_specific_location(Config::ServerBlock *server) {
		const std::vector<Config::LocationBlock> locs = server->get_location();
		std::vector<Config::LocationBlock> matched_locations;
		const std::vector<std::string> uri_paths = _http_request_message.get_uri().get_path(); // FIXME or get_request_uri()?
		for (std::vector<Config::LocationBlock>::const_iterator it = locs.begin(); it != locs.end(); it++) {
			const std::string loc_route = it->get_route(); // FIXME root + route?
			std::string searched_uri = ""; //TODO is it const?
			for (size_t i = 0; i < uri_paths.size(); i++)
			{
				searched_uri += path[i];
				if (searched_uri.compare(loc_route) == 0)
					matched_locations.push_back(*it);
			}
		}
		// the last element of matched_locations is the most spefici route.
		if (matched_locations.size() == 0)
			return NULL;
		else
			return matched_locations.back();

		// VISUALISATION
		// uri    : https://wordpress.com/theme/livro
		// uri_paths
		//	  [0] : wordpress // FIXME ask Xiaojing about if .com is there
		//	  [1] : theme
		//	  [2] : livro

		// CONF LOCS //FIXME discuss the subject : it says no regex! also modifiers in location block?
		// route1 : /    // FIXME this is somehow speacial because it is an umbrella route for every uri?
		// route2 : /wordpress/
		// route3 : /wordpress/theme
		// route5 : /wordpress/theme/livro
		// route6 : /wordpress/theme/livro/support
		// route7 : /wordpress/domains

		// What about root + route?
		// root = /var/www/localhost;
		// root + route = /var/www/localhost//wordpress/theme/livro
		// But we separated the request uri and got rid of the /var/www/? IF so, delete it.
	}

	bool RequestHandler::_verify_method(const std::vector<std::string> methods) {
		const std::string request_method = _http_request_message.get_method();
		for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); it++) {
			if (request_method.compare(*it) == 0)
				return true;
		}
		return false;
	}
}

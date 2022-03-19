#include "ResponseHandler.hpp"
#include "../Utility/Utility.hpp"

#include <sstream> // for converting int to string

#include "../globals.hpp"
// #include "../config/ConfigData.hpp"
// #include "../config/ServerBlock.hpp"

namespace HTTPResponse {
	ResponseHandler::ResponseHandler(HTTPRequest::RequestMessage* request_message, ResponseMessage* response_message)
	: _http_request_message(request_message)
	, _http_response_message(response_message)
	{
	} //TODO do i need a copy const?

	ResponseHandler::~ResponseHandler(){}

	void ResponseHandler::create_http_response() {
		//FIXME what to do when we catch request parser errors like bad request? Do we serve an error page back as a response? If so check the status code here?

		// checks before normal HTTP method handling
		if(!_verify_method(_config.get_limit_except()))
			return(_handle_error(MethodNotAllowed));

		if (!_check_client_body_size())
			return(_handle_error(ContentTooLarge));
		//else //building the response
			//handle_methods?; inside get_and_head, is_cgi, post, delete


		// if(status_code > 300 ?) //this errors are catched after handling methods?
			//build error page (when building first check if error_page exist else build_default_error_page)
		// if(not_redirectied)
			//build response
		return ;
	}

	void ResponseHandler::_handle_error(HTTPResponse::StatusCode code)
	{
		_http_response_message->set_status_code(convert_status_code_to_string(static_cast<int>(code)));
		_http_response_message->set_reason_phrase(HTTPResponse::get_reason_phrase(code));

		if (code == MethodNotAllowed)
			_http_response_message->set_header_element("Allow", _config.get_methods_line());

		//handle custom error pages

		// generate error page
		_http_response_message->set_header_element("Content-Type", "text/html");
		_http_response_message->set_message_body(std::string("<h1>")
								+ _http_response_message->get_status_code() + "</h1><p>"
								+ _http_response_message->get_reason_phrase() + "</p>");

		_build_final_response();
	}

	void ResponseHandler::_build_final_response()
	{
		std::string response;
		std::string msg_body = _http_response_message->get_message_body();

		// set any remaining headers
		_http_response_message->set_header_element("Server", "HungerWeb/1.0");
		_http_response_message->set_header_element("Date", Utility::get_formatted_date());
		// _http_response_message->set_header_element("Content-Length", msg_body.size().c_str()); //FIXME

		// build status line
		response += _http_response_message->get_HTTP_version() + " ";
		response += _http_response_message->get_status_code() + " ";
		response += _http_response_message->get_reason_phrase() + "\r\n";

		// add all the headers to response. Format is {Header}: {Header value} \r\n
		for (std::map<std::string, std::string>::const_iterator it = _http_response_message->get_response_headers().begin(); it != _http_response_message->get_response_headers().end(); it++) {
			if (!it->first.empty())
				response += it->first + ": " + it->second;
			response += "\r\n";
		}

		// if body is not empty add it to  response. Format: \r\n {body}
		response += "\r\n";
		if(!msg_body.empty()) //TODO don't forget to clear body if method is HEAD
			response += msg_body;

		//final step
		_http_response_message->append_complete_response(response);
	}

	bool ResponseHandler::_verify_method(const std::vector<std::string> methods) {
		if (methods.empty())
			return true;
		const std::string request_method = _http_request_message->get_method();
		for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); it++)
			if (request_method.compare(*it) == 0)
				return true;
		return false;
	}

	bool ResponseHandler::_check_client_body_size() {
		int body_size = _http_request_message->get_message_body().size(); //TODO what does size() return
		if (body_size > _config.get_client_max_body_size())
			return false;
		return true;
	}

	const std::string ResponseHandler::convert_status_code_to_string(const int code) {
		std::string stringified_code;
		std::stringstream sstream;
		sstream << code;
		sstream >> stringified_code;
		return stringified_code;
	}

	void ResponseHandler::set_config_rules(const Config::ServerBlock *virtual_server, const Config::LocationBlock *location) {

		_config.set_root_value(virtual_server->get_root());
		_config.set_return_value(virtual_server->get_return());
		_config.set_error_page_value(virtual_server->get_error_page());
		_config.set_client_max_body_size(virtual_server->get_client_max_body_size());
		if (location) //this replaces ServerBlock's config rules with LocationBlock's
		{
			_config.set_specific_location(true);
			_config.set_limit_except(location->get_limit_except());
			_config.set_methods_line(location->get_limit_except());
			_config.set_route(location->get_route());
			_config.set_autoindex(location->get_autoindex());
			_config.set_root_value(location->get_root()); //does it replace it? if it's empty?
			_config.set_return_value(location->get_return()); // does this add to values or replaces them
			_config.set_error_page_value(location->get_error_page());
			if (!location->get_is_size_default())
				_config.set_client_max_body_size(location->get_client_max_body_size()); //TODO how do we know if loc has clientmax
		}
	}
}

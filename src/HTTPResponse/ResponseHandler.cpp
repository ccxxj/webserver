#include "ResponseHandler.hpp"

#include <sstream> // for converting int to string

#include "../globals.hpp"
// #include "../config/ConfigData.hpp"
// #include "../config/ServerBlock.hpp"

namespace HTTPResponse {
	ResponseHandler::ResponseHandler(HTTPRequest::RequestMessage* request_message, ResponseMessage* response_message)
	: _http_request_message(request_message)
	, _http_response_message(response_message)
	{
		(void)_http_request_message; //TODO to silence flags. remove it.
		(void)_http_response_message; //TODO to silence flags. remove it.
	} //TODO do i need a copy const?

	ResponseHandler::~ResponseHandler(){}

	void ResponseHandler::create_http_response() {
		std::cout << "inside create" << std::endl;
		if (_config.has_specific_location()) {
			std::cout << "we have locccc matched" << std::endl;
			if(!_verify_method(_config.get_limit_except())) { //TODO have this in one separate function?
				handle_status_line(MethodNotAllowed);
				std::cout << _http_response_message->get_status_line() << std::endl;
				std::pair<std::string, std::string> header_field("Allow", _config.get_methods_line());
				_http_response_message->set_header_element(header_field);
			}
		}
		if (!_check_client_body_size())
			handle_status_line(ContentTooLarge);
		// checks & building the response
	}

	bool ResponseHandler::_verify_method(const std::vector<std::string> methods) {
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

	void ResponseHandler::handle_status_line(HTTPResponse::StatusCode code) {
		_http_response_message->set_status_code(convert_status_code_to_string(static_cast<int>(code)));
		_http_response_message->set_reason_phrase(HTTPResponse::get_reason_phrase(code));
		std::string status_line = _http_response_message->get_HTTP_version() + " ";
		status_line += _http_response_message->get_status_code() + " ";
		status_line += _http_response_message->get_reason_phrase(); + "\r\n\r\n";
		_http_response_message->set_status_line(status_line);
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

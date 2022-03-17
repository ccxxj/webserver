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
		if (_location) {
			std::cout << "we have locccc matched" << std::endl;
			if(!_verify_method(_location->get_limit_except())) {
				handle_status_line(MethodNotAllowed);
				std::cout << _http_response_message->get_status_line() << std::endl;
			}
		}
		// checks & building the response
	}

	bool ResponseHandler::_verify_method(const std::vector<std::string> methods) {
		const std::string request_method = _http_request_message->get_method();
		for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); it++) {
			if (request_method.compare(*it) == 0)
				return true;
		}
		return false;
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

	void ResponseHandler::set_server(const Config::ServerBlock* server) {
		_server = server;
	}

	void ResponseHandler::set_location(const Config::LocationBlock* location) {
		_location = location;
	}

	void ResponseHandler::set_server_and_location(const Config::ServerBlock *virtual_server, const Config::LocationBlock *location) {
		set_server(virtual_server);
		set_location(location);
	}
}

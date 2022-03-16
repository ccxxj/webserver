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
        // if (!location)
        //     _verify_method(location->get_limit_except()); //HTTPResponse::MethodNotAllowed ? how to move from here?
        // checks & building the response
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
	// bool ResponseHandler::_verify_method(const std::vector<std::string> methods) {
	// 	const std::string request_method = _http_request_message.get_method();
	// 	for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); it++) {
	// 		if (request_method.compare(*it) == 0)
	// 			return true;
	// 	}
	// 	return false;
	// }
}

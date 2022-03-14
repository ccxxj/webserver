#include "ResponseHandler.hpp"

#include <sstream> // for converting int to string

#include "../globals.hpp"
// #include "../config/ConfigData.hpp"
// #include "../config/ServerBlock.hpp"

namespace HTTPResponse {
	ResponseHandler::ResponseHandler(ResponseMessage& response_message)
	// : _http_request_message()
	: _http_response_message(response_message)
	{
	}

	ResponseHandler::~ResponseHandler(){}

	void ResponseHandler::create_http_response(const Config::ServerBlock *virtual_server, const Config::LocationBlock *location) {
		(void)virtual_server;
		(void)location;
        // if (!location)
        //     _verify_method(location->get_limit_except()); //HTTPResponse::MethodNotAllowed ? how to move from here?
        // checks & building the response 
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

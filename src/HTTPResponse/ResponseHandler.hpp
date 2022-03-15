#pragma once

#include <string>


#include "StatusCodes.hpp"
// #include "../config/ServerBlock.hpp"
// #include "../config/LocationBlock.hpp"
// #include "../HTTPRequest/RequestMessage.hpp"
#include "ResponseMessage.hpp" //webserv_response/src/HTTPResponse/

namespace HTTPResponse {
    class ResponseHandler {
    private:
        // HTTPRequest::RequestMessage _http_request_message; //TODO have this and cgi as well.
        ResponseMessage _http_response_message; // smart pointer because I want to change the real response msg cmoning from request handler!
		// Config::ServerBlock ?;
        // Config::LocationBlock ?;

        // void _handle_request_exception(StatusCode code);
        // const std::string _convert_status_code_to_string(const int code);

    public:
        ResponseHandler(ResponseMessage& response_message);
        ~ResponseHandler();

        void create_http_response(const Config::ServerBlock *virtual_server, const Config::LocationBlock *location);
    };
}

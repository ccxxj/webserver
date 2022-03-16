#pragma once

#include <string>


#include "StatusCodes.hpp"
#include "../HTTPRequest/RequestMessage.hpp"
// #include "../config/ServerBlock.hpp"
// #include "../config/LocationBlock.hpp"
//
#include "ResponseMessage.hpp" //webserv_response/src/HTTPResponse/

namespace HTTPResponse {
    class ResponseHandler {
    private:
        HTTPRequest::RequestMessage* _http_request_message; //TODO have this and cgi as well.
        ResponseMessage* _http_response_message; //TODO check if you change the real response msg cmoning from request handler!
		Config::ServerBlock* _server;
        Config::LocationBlock* _location;

        // void _handle_request_exception(StatusCode code);
        // const std::string _convert_status_code_to_string(const int code);

    public:
        ResponseHandler(ResponseMessage* response_message);
        ~ResponseHandler();

        void create_http_response(const Config::ServerBlock *virtual_server, const Config::LocationBlock *location);

		void set_server(Config::ServerBlock* server);
		void set_location(Config::LocationBlock* location);
		//TODO do i need getter if I think i wont need it  at all outside of the class?
    };
}

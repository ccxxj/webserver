#pragma once

#include <string>

#include "RequestHandlerDelegate.hpp"
#include "../HTTPRequest/RequestMessage.hpp"
#include "../HTTPResponse/ResponseMessage.hpp"
#include "../HTTPRequest/RequestParser.hpp"
#include "../HTTPResponse/StatusCodes.hpp"

namespace HTTP {
    class RequestHandler
    {
    private:
        HTTPRequest::RequestMessage _http_request_message;
        HTTPResponse::ResponseMessage _http_response_message;
        RequestHandlerDelegate& _delegate;
        HTTPRequest::RequestParser _parser;

        void _handle_request_exception(HTTPResponse::StatusCode code);
        const std::string _convert_status_code_to_string(const int code);
        
    public:
        RequestHandler(RequestHandlerDelegate& delegate);
        ~RequestHandler();
        void handle_http_request();
    };
}

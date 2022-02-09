#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include <string>

#include "../HTTPRequest/RequestMessage.hpp"
#include "../HTTPResponse/ResponseMessage.hpp"
#include "../HTTPResponse/StatusCodes.hpp"
#include "Connection.hpp"

namespace HTTP {
    class RequestHandler
    {
    private:
        HTTPRequest::RequestMessage _http_request_message;
        HTTPResponse::ResponseMessage _http_response_message;
        Connection _connection;
        
        void _handle_exception(HTTPResponse::StatusCode code);
        const std::string _convert_status_code_to_string(const int code);
        
    public:
        RequestHandler(const Connection& active_connection);
        ~RequestHandler();
        void handle_http_request();
    };
}

#endif
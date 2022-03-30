#pragma once

#include <string>

#include "RequestHandlerDelegate.hpp"
#include "../HTTPRequest/RequestMessage.hpp"
#include "../HTTPResponse/ResponseMessage.hpp"
#include "../HTTPRequest/RequestParser.hpp"
#include "../HTTPResponse/StatusCodes.hpp"
#include "../HTTPResponse/ResponseHandler.hpp"
#include "../config/ConfigData.hpp"
#include "ServerStructs.hpp"

namespace HTTP {
    class RequestHandler
    {
    private:
        HTTPRequest::RequestMessage _http_request_message;
        HTTPResponse::ResponseMessage _http_response_message;
        RequestHandlerDelegate& _delegate;
        HTTPRequest::RequestParser _parser;
		Config::ConfigData *_config_data;
		ListenInfo& _connection_listen_info; //added for host port match
        HTTPResponse::ResponseHandler response_handler;

        void _handle_request_exception(HTTPResponse::StatusCode code);
        const std::string _convert_status_code_to_string(const int code);
        void _process_http_request(int kq);
		const Config::ServerBlock* _find_virtual_server();
		const Config::ServerBlock* _match_server_based_on_server_name(std::vector<const Config::ServerBlock*> matching_servers);
		const Config::LocationBlock* _match_most_specific_location(const Config::ServerBlock *server);

    public:
        RequestHandler(RequestHandlerDelegate& delegate, Config::ConfigData *config_data, ListenInfo& listen_info);
        ~RequestHandler();
        void handle_http_request(int kq);
    };
}

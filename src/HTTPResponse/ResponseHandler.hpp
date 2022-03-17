#pragma once

#include <string>

#include "StatusCodes.hpp"
#include "../config/ServerBlock.hpp"
#include "../HTTPRequest/RequestMessage.hpp"
#include "ResponseMessage.hpp"

namespace HTTPResponse
{
	class ResponseHandler
	{
	private:
		HTTPRequest::RequestMessage *_http_request_message;
		ResponseMessage *_http_response_message; // TODO check if you change the real response msg cmoning from request handler!
		const Config::ServerBlock *_server;
		const Config::LocationBlock *_location;

		// void _handle_request_exception(StatusCode code);
		// const std::string _convert_status_code_to_string(const int code);
		bool _verify_method(const std::vector<std::string> methods);
		const std::string& _create_allowed_methods_line(const std::vector<std::string> methods);

	public:
		ResponseHandler(HTTPRequest::RequestMessage *request_message, ResponseMessage *response_message);
		~ResponseHandler();

		void create_http_response();
		void handle_status_line(HTTPResponse::StatusCode code);
		const std::string convert_status_code_to_string(const int code);
		void set_server(const Config::ServerBlock *server);
		void set_location(const Config::LocationBlock *location);
		void set_server_and_location(const Config::ServerBlock *virtual_server, const Config::LocationBlock *location);
		// TODO do i need getter if I think i wont need it  at all outside of the class?
	};
}

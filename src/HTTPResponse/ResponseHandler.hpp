#pragma once

#include <string>

#include "StatusCodes.hpp"
#include "../config/ServerBlock.hpp"
#include "../HTTPRequest/RequestMessage.hpp"
#include "../Utility/File.hpp"
#include "ResponseMessage.hpp"
#include "SpecifiedConfig.hpp"

namespace HTTPResponse
{
	class ResponseHandler
	{
	private:
		HTTPRequest::RequestMessage *_http_request_message;
		ResponseMessage *_http_response_message;
		SpecifiedConfig _config;
		Utility::File _file;

		bool _verify_method(const std::vector<std::string> methods);
		const std::string& _create_allowed_methods_line(const std::vector<std::string> methods);
		bool _check_client_body_size();
		void _handle_methods(void);
		void _serve_file(void);
		void _serve_directory(void);
		void _serve_found_file(const std::string &str);
		bool _search_for_index_page();
		void _delete_file(void);
		void _upload_file(void);
		void _build_final_response();
		std::string _response_status();

	public:
		ResponseHandler(HTTPRequest::RequestMessage *request_message, ResponseMessage *response_message);
		ResponseHandler(const ResponseHandler& other);
		const ResponseHandler &operator=(const ResponseHandler &other);
		~ResponseHandler();

		void create_http_response();
		void handle_error(HTTPResponse::StatusCode code);
		void set_config_rules(const Config::ServerBlock *virtual_server, const Config::LocationBlock *location);
	};
}

#pragma once

#include <string>

#include "StatusCodes.hpp"
#include "../config/ServerBlock.hpp"
#include "../HTTPRequest/RequestMessage.hpp"
#include "../Utility/File.hpp"
#include "ResponseMessage.hpp"
#include "SpecifiedConfig.hpp"
#include "../CGI/CGIHandler.hpp"

namespace HTTPResponse
{
	class ResponseHandler
	{
	private:
		HTTPRequest::RequestMessage *_http_request_message;
		ResponseMessage *_http_response_message;
		SpecifiedConfig _config;
		Utility::File _file;
		CGIHandler _cgi_handler;
		size_t _redirection_loop;

		bool _verify_method(const std::vector<std::string> methods);
		const std::string& _create_allowed_methods_line(const std::vector<std::string> methods);
		bool _check_client_body_size();
		void _handle_methods(void);
		void _serve_file(void);
		void _serve_directory(void);
		void _serve_found_file(const std::string &str);
		void _serve_custom_error_page(const std::string &str);
		bool _search_for_index_page();
		void _delete_file(void);
		void _upload_file(void);
		void _build_final_response();
		void _build_final_cgi_response(std::string &cgi_response);
		void _handle_redirection();


	public:
		ResponseHandler(HTTPRequest::RequestMessage *request_message, ResponseMessage *response_message);
		ResponseHandler(const ResponseHandler& other);
		const ResponseHandler &operator=(const ResponseHandler &other);
		~ResponseHandler();

		void create_http_response(int kq);
		void handle_error(HTTPResponse::StatusCode code);
		char* handle_cgi(int fd, int kq);
		void set_config_rules(const Config::ServerBlock *virtual_server, const Config::LocationBlock *location);
		std::string response_status();
		std::string request_info();
	};
}

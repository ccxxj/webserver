#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <exception>

#include "../HTTPRequest/RequestMessage.hpp"
#include "../HTTPResponse/SpecifiedConfig.hpp"
#include "../Constants.hpp"

class CGIHandler
{
private:
	char *_envp[Constants::ENVP_SIZE];
	char *_argument[Constants::ARGUMENTS_SIZE];
	std::map<std::string, std::string> _meta_variables;
	std::string _cgi_name;
	std::vector<std::string> _cgi_extention;
	bool _search_cgi_extension;
	int _input_pipe[2];
	int _output_pipe[2];
	int _socket_fd;
	std::string _response;
	std::string _request_message_body;

	void update_path_translated(void);
	void initialize_cgi_arguments();
	class CGIexception : public std::exception{
		const char* what() const _NOEXCEPT { return "internal server error"; }
	};
	CGIHandler();

public:		
	CGIHandler(int port_number);
	~CGIHandler();
	void parse_meta_variables(HTTPRequest::RequestMessage *_http_request_message, HTTPResponse::SpecifiedConfig &_config);
	void prepare_cgi_data(HTTPRequest::RequestMessage *_http_request_message, HTTPResponse::SpecifiedConfig &_config, int socket_fd);
	void search_cgi(std::vector<std::string> &path);
	void set_envp(void);
	void set_argument(std::string cgi_name);
	void set_response_message_body(std::string str);
	int get_read_fd() const;
	int get_write_fd() const;
	int get_socket_fd() const;
	std::string get_response_message_body();
	std::string get_request_message_body();
	bool get_search_cgi_extention_result() const;
	void execute_cgi(int kq);
};

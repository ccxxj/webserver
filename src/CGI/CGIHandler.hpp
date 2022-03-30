#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../HTTPRequest/RequestMessage.hpp"
#include "../HTTPResponse/SpecifiedConfig.hpp"

class CGIHandler
{
private:
	char *_envp[21];				// TODO check if this is the right size, terminated with 0
	char *_argument[2];
	std::map<std::string, std::string> _meta_variables;
	std::string _cgi_name;
	std::vector<std::string> _cgi_extension;
	bool _search_cgi_extension;
	void update_path_translated(void);

public:
	CGIHandler();
	~CGIHandler();
	// void parse_meta_variables(HTTPRequest::RequestMessage request_message);//TODO update later
	void parse_meta_variables(HTTPRequest::RequestMessage *_http_request_message, HTTPResponse::SpecifiedConfig &_config);
	void search_cgi(std::vector<std::string> &path);
	// void parse_meta_variables(void);
	void set_envp(void);
	void set_argument(std::string cgi_name);
	// char **get_argument();
	char* execute_cgi(HTTPRequest::RequestMessage *_http_request_message, HTTPResponse::SpecifiedConfig &_config, int fd, int kq);
	// int execute_cgi(HTTPRequest::RequestMessage *_http_request_message, HTTPResponse::SpecifiedConfig &_config);
};

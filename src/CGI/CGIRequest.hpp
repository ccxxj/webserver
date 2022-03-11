#pragma once

#include <iostream>
#include <map>
#include <string>

// #include "../HTTPRequest/RequestMessage.cpp"

class CGIRequest
{
private:
	// meta_variable [case insensative for the variable name, case sensative for the variable value]
	// if value is not provided or not the expected value, respond: invalid Server_Protocal version => no further execution of the program, and no response send back???? => TODO think later
	// std::string AUTH_TYPE;		// NA, identify any mechanism used by the server to authenticate the user
	// std::string CONTENT_LENGTH; // the size of the message body
	// std::string CONTENT_TYPE;
	// std::string GATEWAY_INTERFACE;
	// std::string PATH_INFO; // TODO important to have. find out where (uri?) //not URI-encoded => TODO do not decode if detect it is related to cgi (.cgi, .php, .py)
	// std::string PATH_TRANSLATED;
	// std::string QUERY_STRING; // TODO also uri??
	// std::string REMOTE_ADDR;
	// std::string REMOTE_HOST;
	// std::string REMOTE_IDENT;
	// std::string REMOTE_USER;
	// std::string REQUEST_METHOD; // input from RequestMessage.get_method()
	// std::string SCRIPT_NAME;
	// std::string SERVER_NAME;
	// std::string SERVER_PORT;
	// std::string SERVER_PROTOCOL; // input from RequestMessage.get_HTTP_version()
	// std::string SERVER_SOFTWARE;
	// std::string scheme;				// not sure if needed here
	// std::string protocol_var_name;	// not sure if needed here
	// std::string extension_var_name; // not sure if needed here
	char *_envp[21];				// TODO check if this is the right size, terminated with 0
	char *_argument[2];
	std::map<std::string, std::string> _meta_variables;
	std::string _uri;

public:
	CGIRequest();
	~CGIRequest();
	// void parse_meta_variables(HTTPRequest::RequestMessage request_message);//TODO update later
	void parse_meta_variables(void);
	void set_envp(void);
	void set_argument(std::string cgi_path);
	// char **get_argument();
	void execute_cgi();
};
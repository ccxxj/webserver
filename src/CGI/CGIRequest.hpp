#pragma once

class CGIRequest{
private:
//meta_variable [case insensative]
	std::string AUTH_TYPE;
	std::string CONTENT_LENGTH;	
	std::string CONTENT_TYPE;	
	std::string GATEWAY_INTERFACE;	
	std::string PATH_INFO;	
	std::string PATH_TRANSLATED;	
	std::string QUERY_STRING;	
	std::string REMOTE_ADDR;	
	std::string REMOTE_HOST;	
	std::string REMOTE_IDENT;	
	std::string REMOTE_USER;	
	std::string REQUEST_METHOD;	
	std::string SCRIPT_NAME;	
	std::string SERVER_NAME;	
	std::string SERVER_PORT;	
	std::string SERVER_PROTOCOL;	
	std::string SERVER_SOFTWARE;	
	std::string scheme;//not sure if needed here
	std::string protocol_var_name;//not sure if needed here
	std::string extension_var_name;//not sure if needed here


};
#include "CGIRequest.hpp"

#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>


CGIRequest::CGIRequest(){
//initialize the meta_variable map
	_meta_variables["AUTH_TYPE"]; //from standard request fields
	_meta_variables["CONTENT_LENGTH"];//from standard request fields
	_meta_variables["CONTENT_TYPE"];//from standard request fields
	_meta_variables["GATEWAY_INTERFACE"] = "CGI/1.1"; //not sure TODO
	_meta_variables["PATH_INFO"]; //portion of uri, the segment after identify the script itself (run DECODE!!). (cgi/more => /more). can be void
	_meta_variables["PATH_TRANSLATED"]; // if path_info is null, path_translated is null. otherwise: root + path_info
	_meta_variables["QUERY_STRING"];//from uri query string map. TODO check if the key is set, is the default value null?
	_meta_variables["REMOTE_ADDR"];//set to the server network address. can be void
	_meta_variables["REMOTE_HOST"]; //if not remote_host value provided (hostname), substitute with the remote_address value
	_meta_variables["REMOTE_IDENT"];
	_meta_variables["REMOTE_USER"];
	_meta_variables["REQUEST_METHOD"];// from method
	_meta_variables["SCRIPT_NAME"]; //from uri
	_meta_variables["SERVER_NAME"];
	_meta_variables["SERVER_PORT"];
	_meta_variables["SERVER_PROTOCOL"];
	_meta_variables["SERVER_SOFTWARE"];
	// _meta_variables["scheme"];//not sure if needed, as it will always be http
	// _meta_variables["protocol_var_name"];//not sure if needed
	// _meta_variables["extension_var_name"];//not sure if needed

//initialize the extensions to search for
	_cgi_extension.push_back(".php");
	_cgi_extension.push_back("cgi");
	_cgi_extension.push_back(".py");
	_cgi_extension.push_back(".pl");
}

CGIRequest::~CGIRequest(){
//free all the _envp[21] (cuz I used strdup for every elements)
	// if(_search_cgi_extension){
	// 	for(int i = 0; i < 20; i++){
	// 		if(_envp[i])
	// 			free(_envp[i]);
	// 	}
	// }
}

void CGIRequest::parse_meta_variables(HTTPRequest::RequestMessage *_http_request_message, HTTPResponse::SpecifiedConfig _config)
// void CGIRequest::parse_meta_variables(void)
{
	//check with Olga to map to the request message class
	//TODO this need to be updated to the actual env variable, for now I am setting up myself for the basic to run cgi
	_meta_variables["SERVER_PROTOCOL"] = "HTTP/1.0";
	_meta_variables["REQUEST_METHOD"] = "GET";

//actual data
	_meta_variables["AUTH_TYPE"] = _http_request_message->get_header_value("AUTHORIZATION");
	_meta_variables["CONTENT_LENGTH"] = _http_request_message->get_header_value("CONTENT_LENGTH");
	_meta_variables["CONTENT_TYPE"] = _http_request_message->get_header_value("CONTENT_TYPE");
	_meta_variables["GATEWAY_INTERFACE"] = "CGI/1.1"; //not sure TODO
	// _meta_variables["PATH_INFO"]; //portion of uri, the segment after identify the script itself (run DECODE!!). (cgi/more => /more). can be void
	// _meta_variables["PATH_TRANSLATED"]; // if path_info is null, path_translated is null. otherwise: root + path_info
	// _meta_variables["QUERY_STRING"];//from uri query string map. TODO check if the key is set, is the default value null?
	// _meta_variables["REMOTE_ADDR"];//set to the server network address. can be void
	// _meta_variables["REMOTE_HOST"]; //if not remote_host value provided (hostname), substitute with the remote_address value
	// _meta_variables["REMOTE_IDENT"];
	// _meta_variables["REMOTE_USER"];
	// _meta_variables["REQUEST_METHOD"] = request_message.get_method();// from method
	// _meta_variables["SCRIPT_NAME"]; //from uri
	// _meta_variables["SERVER_NAME"];//from config
	// _meta_variables["SERVER_PORT"];//from config
	// _meta_variables["SERVER_PROTOCOL"];
	// _meta_variables["SERVER_SOFTWARE"];
}

void CGIRequest::set_envp(void)
{
	std::string temp;
	int i = 0;
	std::map<std::string, std::string>::iterator it = _meta_variables.begin();
	while(it != _meta_variables.end()){
		temp = it->first + "=" + it->second + "\0";
		it++;
		_envp[i] = strdup(temp.c_str());
		i++;
	}
	_envp[i] = NULL;
}

void CGIRequest::set_argument(std::string cgi_path)
{
	_argument[0] = strdup(cgi_path.c_str());
	_argument[1] = NULL;//TODO is it always NULL?
}

void CGIRequest::search_cgi(std::string uri)
{
	std::vector<std::string>::iterator it = _cgi_extension.begin();
	int position_path_info, position_cgi;
	while(it != _cgi_extension.end()){
		position_cgi = uri.find(*it);
		if( position_cgi != (int)std::string::npos){
			position_path_info = position_cgi + (*it).length();
			int len = uri.length() - position_path_info;
			if((*it)[position_path_info] == '/' && len > 0)
				_meta_variables["PATH_INFO"] = uri.substr(position_path_info, len);//TODO validation on the path_info, also the decoding. 
			_search_cgi_extension = true;
			return;
		}
		it++;
	}
	_search_cgi_extension = false;
}

int CGIRequest::execute_cgi(HTTPRequest::RequestMessage *_http_request_message, HTTPResponse::SpecifiedConfig _config, int fd)
{
	// search_cgi("/cgi_tester/./");
	search_cgi("/first.cgi/./");
	int fd2 = open("test", O_WRONLY|O_CREAT|O_TRUNC, 755);
	if(fd2 < 0)
		std::cout << "open fail\n";
	// std::cout << "the result of search: " << _search_cgi_extension << std::endl;
	if(_search_cgi_extension == false) //input need to change to uri
		return 0;
	pid_t pid;
	set_argument("first.cgi");
	parse_meta_variables(_http_request_message, _config);
	set_envp();
	pid = fork();
	if(pid == 0){
		if(dup2(fd2, 1) == -1)
			std::cout << "dup fail\n";
		std::cout << "child process \n";
		if(execve(_argument[0], _argument, _envp) == -1)
			return -1;
		std::cerr << "WHY AM I HERE?\n";
	}
	else if(pid < (pid_t)0){
		std::cout << "fork failed\n"; //TODO decide exception to throw later
		return -1;
	}
	else{
		wait(0);
		char buf[4086];
		read(fd2, buf, sizeof(buf));//TODO find a better function
		//the buf need to goes into response
		std::cout << buf << "\n";
	}
	// int fd[2];
	// if (pipe(fd)){
	// 	std::cout << "pipe failed\n"; //TODO decide exception to throw later
	// }
	// pid_t pid;
	// set_argument("cgi_tester");
	// parse_meta_variables(_http_request_message, _config);
	// set_envp();
	// pid = fork();
	// if(pid == 0){
	// 	std::cout  << "fd0: " << fd[1] << "fd0: " << fd[0] << " child process \n";
	// 	dup2(fd[1], 1);
	// 	close(fd[0]);
	// 	std::cout << "child process \n";
	// 	if(execve(_argument[0], _argument, _envp) == -1)
	// 		return -1;
	// }
	// else if(pid < (pid_t)0){
	// 	std::cout << "fork failed\n"; //TODO decide exception to throw later
	// 	return -1;
	// }
	// else{
	// 	wait(0);
	// 	close(fd[1]);
	// 	char buf[4086];
	// 	read(fd[0], buf, sizeof(buf));//TODO find a better function
	// 	//the buf need to goes into response
	// 	std::cout << buf << "\n";
	// }
	close(fd2);
	return 1;
}
#include "CGIRequest.hpp"

#include <unistd.h>
#include <string.h>

CGIRequest::CGIRequest(){
//initialize the meta_variable map
	_meta_variables["AUTH_TYPE"];
	_meta_variables["CONTENT_LENGTH"];
	_meta_variables["CONTENT_TYPE"];
	_meta_variables["GATEWAY_INTERFACE"];
	_meta_variables["PATH_INFO"];
	_meta_variables["PATH_TRANSLATED"];
	_meta_variables["QUERY_STRING"];
	_meta_variables["REMOTE_ADDR"];
	_meta_variables["REMOTE_HOST"];
	_meta_variables["REMOTE_IDENT"];
	_meta_variables["REMOTE_USER"];
	_meta_variables["REQUEST_METHOD"];
	_meta_variables["SCRIPT_NAME"];
	_meta_variables["SERVER_NAME"];
	_meta_variables["SERVER_PORT"];
	_meta_variables["SERVER_PROTOCOL"];
	_meta_variables["SERVER_SOFTWARE"];
	_meta_variables["scheme"];//not sure if needed, as it will always be http
	_meta_variables["protocol_var_name"];//not sure if needed
	_meta_variables["extension_var_name"];//not sure if needed
}

CGIRequest::~CGIRequest()
{
//free all the _envp[21] (cuz I used strdup for every elements)
	for(int i = 0; i < 21; i++)
	{
		if(_envp[i])
			free(_envp[i]);
	}
}

// void CGIRequest::parse_meta_variables(HTTPRequest::RequestMessage request_message)
void CGIRequest::parse_meta_variables(void)
{
	//check with Olga to map to the request message class
	//TODO this need to be updated to the actual env variable, for now I am setting up myself for the basic to run cgi
	_meta_variables["SERVER_PROTOCOL"] = "HTTP/1.0";
	_meta_variables["REQUEST_METHOD"] = "GET";
}

void CGIRequest::set_envp(void)
{
	std::string temp;
	int i = 0;
	std::map<std::string, std::string>::iterator it = _meta_variables.begin();
	while(it != _meta_variables.end())
	{
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


void CGIRequest::execute_cgi()
{
	int fd[2];
	if (pipe(fd))
	{
		std::cout << "pipe failed\n"; //TODO decide exception to throw later
	}
	pid_t pid;
	pid = fork();
	set_argument("cgi_tester");
	parse_meta_variables();
	set_envp();
	if(pid == (pid_t)0)
	{
		dup2(fd[1], 1);
		close(fd[0]);
		execve(_argument[0], _argument, _envp);
	}
	else if(pid < (pid_t)0)
	{
		std::cout << "fork failed\n"; //TODO decide exception to throw later
	}
	else
	{
		close(fd[1]);
		char buf[8000];
		read(fd[0], buf, 8000);
		//the buf need to goes into response
		std::cout << buf << "\n";
	}
}

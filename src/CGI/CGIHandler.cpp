#include "CGIHandler.hpp"

#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/event.h>//for kqueue and kevent
// #include <u.h>//for RFPROC
// #include <libc.h>//RFPROC
// #include <signal.h>
// #include <sys/param.h>
// #include <sys/types.h>
// #include <sys/mman.h>
// #include <sys/stat.h>
// #include <semaphore.h>
// #include <fcntl.h>


CGIHandler::CGIHandler(){
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
	_cgi_extension.push_back(".cgi");
	_cgi_extension.push_back(".py");
	_cgi_extension.push_back(".pl");//TODO decide later if we want to let user to decide what are the extension can be executed as cgi
}

CGIHandler::~CGIHandler(){
//free all the _envp[21] (cuz I used strdup for every elements)
	// if(_search_cgi_extension){
	// 	for(int i = 0; i < 20; i++){
	// 		if(_envp[i])
	// 			free(_envp[i]);
	// 	}
	// }
}

/* append the cgi-bin location to the path after the script in the uri */
void CGIHandler::update_path_translated(void){
	if(!_meta_variables["PATH_INFO"].empty())
		_meta_variables["PATH_TRANSLATED"] = "/cgi-bin/" + _meta_variables["PATH_INFO"];
}

void CGIHandler::parse_meta_variables(HTTPRequest::RequestMessage *_http_request_message, HTTPResponse::SpecifiedConfig &_config)
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
	// (has been parsed during cgi searching)_meta_variables["PATH_INFO"];
	update_path_translated();// if path_info is null, path_translated is null. otherwise: root + path_info
	_meta_variables["QUERY_STRING"] = _http_request_message->get_uri().get_query();//TODO change query parse function in URI?? check with team. need a simple string of query
	// _meta_variables["REMOTE_ADDR"];//set to the server network address. can be void
	// _meta_variables["REMOTE_HOST"]; //if not remote_host value provided (hostname), substitute with the remote_address value
	// _meta_variables["REMOTE_IDENT"];
	// _meta_variables["REMOTE_USER"];
	_meta_variables["REQUEST_METHOD"] = _http_request_message->get_method();// from method
	_meta_variables["SCRIPT_NAME"] = "/cgi-bin/" + _cgi_name; //path + script name
	// _meta_variables["SERVER_NAME"];//from config
	// _meta_variables["SERVER_PORT"];//from config
	// _meta_variables["SERVER_PROTOCOL"];
	// _meta_variables["SERVER_SOFTWARE"];
}

void CGIHandler::set_envp(void)
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

void CGIHandler::set_argument(std::string cgi_name)
{
	char *ptr;
	char *buf;
	long size = pathconf(".", _PC_PATH_MAX);
	if((buf = (char *)malloc((size_t)size)) != NULL)
		ptr = getcwd(buf, (size_t)size);
	std::string executable_path(buf);//get the current executable location
	std::string full_path = executable_path + "/cgi-bin/" + cgi_name; //define the default cgi-bin (should be in the same location with the executable)
	_argument[0] = strdup(full_path.c_str());
	_argument[1] = NULL;//TODO is it always NULL?
}

//input parameter will be uriData->get_path()
void CGIHandler::search_cgi(std::vector<std::string> &path){
	int size = path.size();
	for(int i = 0; i < size; i++){
		std::vector<std::string>::iterator it = _cgi_extension.begin();
		while(it != _cgi_extension.end()){
			if(path[i].find(*it) != (unsigned long)std::string::npos){
				_cgi_name = path[i];
				for(int j = i + 1; j < size; j++)
					_meta_variables["PATH_INFO"] += "/" + path[j];
				_search_cgi_extension = true;
				return;
			}
			it++;
		}
	}
	_search_cgi_extension = false;
}

/* can be moved to utility or deleted*/
void print_array(char **envp){
	std::cout << "envp is: ";
	int i = 0;
	while(envp[i]){
		std::cout << envp[i] << std::endl;
		i++;
	}
}

// int CGIHandler::execute_cgi(HTTPRequest::RequestMessage *_http_request_message, HTTPResponse::SpecifiedConfig &_config)
char* CGIHandler::execute_cgi(HTTPRequest::RequestMessage *_http_request_message, HTTPResponse::SpecifiedConfig &_config, int fd, int kq)
{
	std::vector<std::string> path = _http_request_message->get_uri().get_path();
	std::cout << "check segfault\n";
	search_cgi(path);
	std::cout << "search result: " << _search_cgi_extension << std::endl;
	if(_search_cgi_extension == false)
		return nullptr;	
	char *buf = (char *)malloc(4086 * sizeof(char));
	int inputPipe[2], outputPipe[2];
	if(pipe(inputPipe) == -1){
		std::perror("pipe");//TODO create exception later??
	}
	if(pipe(outputPipe) == -1){
		std::perror("pipe");//TODO create exception later??
	}
	set_argument(_cgi_name);//TODO replace by the actual path, currently I am only use the predefined path
	parse_meta_variables(_http_request_message, _config);//TODO replace by input from http request get_message_body
	set_envp();
	std::string request_message_body = _http_request_message->get_message_body();
	std::cout << "message body is " << request_message_body << std::endl;
	write(inputPipe[1], request_message_body.c_str(), request_message_body.size());
	
	/*set up kevent and kqueue to monitor also child process*/
	// struct kevent ev;

	// signal(SIGINT, SIG_IGN);
	// EV_SET(&ev, outputPipe[0], EVFILT_PROC, EV_ADD | EV_ENABLE, NOTE_FORK | NOTE_EXEC, 0, 0);
	
	// EV_SET(&ev, outputPipe[0], EVFILT_PROC, EV_ADD | EV_ENABLE, NULL, 0, 0);
	
	pid_t pid = fork();
	if(pid < 0){
		perror("fork failure");//TODO create exception later??
	}
	else if(pid == 0){
		// if(kevent(kq, &ev, 1, NULL, 0, NULL))
			// perror("kevent");
		std::cout << "child process" << std::endl;
		if(dup2(inputPipe[0], 0) < 0)
			perror("dup failure");
		if(dup2(outputPipe[1], 1) < 0)
			perror("dup failure");
		close(inputPipe[1]);
		close(outputPipe[0]);
		//fcntl() set non-blocking flag??
		if(execve(_argument[0], _argument, _envp) == -1){
			perror("execution error");//TODO create exception later??
			return nullptr;
		}
	}
	else{
		wait(0);//waitpid
		//TODO do I need to close the write end? I think kqueue will take care of it
		//TODO if the process hang due to the execution was hanging, currently it is blocking. implement kqueue would solve the problem? => so it is needed to watch on the child process in this case
		//TODO handle different error case: 1. execution problem 2. the requested cgi does not exist 3?
		//TODO currently when I input localhost as request, there is segmentation fault, to discuss with the team
		// close(outputPipe[1]);
		std::cout << "parent process: " << std::endl;
		memset(buf, 0, 4086);
		ssize_t return_size = read(outputPipe[0], buf, 4086);
		// std::cout <<"return size: " << return_size << "msg " << buf << std::endl;
		close(outputPipe[0]);
	}
	return buf;
}

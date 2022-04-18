#include "CGIHandler.hpp"

#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/event.h>//for kqueue and kevent
#include <sys/types.h>
#include <sys/stat.h>

#include "../Utility/Utility.hpp"
#include "../Constants.hpp"


CGIHandler::CGIHandler(){
//initialize the meta_variable map
	_meta_variables["AUTH_TYPE"] = "";
	_meta_variables["CONTENT_LENGTH"] = "";
	_meta_variables["CONTENT_TYPE"] = "";
	_meta_variables["GATEWAY_INTERFACE"] = "CGI/1.1";
	_meta_variables["PATH_INFO"] = "";
	_meta_variables["PATH_TRANSLATED"] = "";
	_meta_variables["QUERY_STRING"] = "";
	_meta_variables["REMOTE_ADDR"] = "";
	_meta_variables["REMOTE_HOST"] = "";
	_meta_variables["REMOTE_IDENT"] = ""; // not applicable in our server (not necessary feature according to the RFC)
	_meta_variables["REMOTE_USER"] = "";
	_meta_variables["REQUEST_METHOD"] = "";
	_meta_variables["SCRIPT_NAME"] = "";
	_meta_variables["SERVER_NAME"] = "";
	_meta_variables["SERVER_PORT"] = "";
	_meta_variables["SERVER_PROTOCOL"] = "";
	_meta_variables["SERVER_SOFTWARE"] = "";

	_input_pipe[0] = -1;
	_input_pipe[1] = -1;
	_output_pipe[0] = -1;
	_output_pipe[1] = -1;

	_response = "";
	initialize_cgi_arguments();
}

CGIHandler::~CGIHandler(){
	for (int i = 0; i < Constants::ENVP_SIZE; i++) {
		free(_envp[i]);
	}
	for (int i = 0; i < Constants::ARGUMENTS_SIZE; i++) {
		free(_argument[i]);
	}
}

void CGIHandler::initialize_cgi_arguments() {
	for (int i = 0; i < Constants::ENVP_SIZE; i++) {
		_envp[i] = NULL;
	}
	for (int i = 0; i < Constants::ARGUMENTS_SIZE; i++) {
		_argument[i] = NULL;
	}
}

/* append the cgi-bin location to the path after the script in the uri */
void CGIHandler::update_path_translated(void){
	if(!_meta_variables["PATH_INFO"].empty())
		_meta_variables["PATH_TRANSLATED"] = "/cgi-bin/" + _meta_variables["PATH_INFO"];
}

void CGIHandler::parse_meta_variables(HTTPRequest::RequestMessage *_http_request_message, HTTPResponse::SpecifiedConfig &_config)
{
	//TODO this need to be updated to the actual env variable, for now I am setting up myself for the basic to run cgi
	_meta_variables["SERVER_PROTOCOL"] = "HTTP/1.1";

//actual data
	std::string authorization = "";
	if (_http_request_message->has_header_field("AUTHORIZATION")) {
		authorization= _http_request_message->get_header_value("AUTHORIZATION");
		std::vector<std::string> authorisation_parts = Utility::_split_line_in_two(authorization, ' ');
		if (authorisation_parts.size() > 0) {
			_meta_variables["AUTH_TYPE"] = authorisation_parts[0];
			if (authorisation_parts.size() > 1) {
				_meta_variables["REMOTE_USER"] = authorisation_parts[1];
			}
		}
	}
	if (_http_request_message->has_header_field("CONTENT_LENGTH")) {
		_meta_variables["CONTENT_LENGTH"] = _http_request_message->get_header_value("CONTENT_LENGTH");
	}
	if (_http_request_message->has_header_field("CONTENT_TYPE")) {
		_meta_variables["CONTENT_TYPE"] = _http_request_message->get_header_value("CONTENT_TYPE");
	}
	_meta_variables["GATEWAY_INTERFACE"] = "CGI/1.1"; //not sure TODO
	// _meta_variables["PATH_INFO"] = "/cgi-bin/" + _cgi_name;//this is contradicting with the RFC
	_meta_variables["PATH_INFO"] = "/directory/youpi.bla";
	update_path_translated();// if path_info is null, path_translated is null. otherwise: root + path_info
	_meta_variables["PATH_TRANSLATED"] = _meta_variables["PATH_INFO"];//TODO REMOVE LATER THIS IS ONLY FOR CGI_TESTER
	_meta_variables["QUERY_STRING"] = _http_request_message->get_uri().get_query();
	_meta_variables["REMOTE_ADDR"] = "127.0.0.1";//TODO @Irem//set to the server network address. can be void
	if (_http_request_message->has_header_field("REMOTE_HOST")) {
		_meta_variables["REMOTE_HOST"] = _http_request_message->get_header_value("HOST"); //TODO @Irem//if not remote_host value provided (hostname), substitute with the remote_address value
	}
	_meta_variables["SERVER_NAME"] = _meta_variables["REMOTE_HOST"];							
	_meta_variables["REQUEST_METHOD"] = _http_request_message->get_method();// from method
	_meta_variables["SCRIPT_NAME"] = "/cgi-bin/" + _cgi_name; //path + script name
	// _meta_variables["SCRIPT_NAME"] = "/directory/youpi.bla"; //TODO change back later
	// _meta_variables["SERVER_PORT"];//TODO @Irem
	_meta_variables["SERVER_PROTOCOL"] = "HTTP/1.1";
	_meta_variables["SERVER_SOFTWARE"] = "HungerWeb 1.0";//TODO @Irem
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
}

void CGIHandler::set_argument(std::string cgi_name)
{
	char *cwd = getcwd(NULL, 0); // get the current executable location
	std::string full_path = std::string(cwd) + "/cgi-bin/" + cgi_name; //define the default cgi-bin (should be in the same location with the executable)
	free(cwd);					  
	_argument[0] = strdup(full_path.c_str());
}

//input parameter will be uriData->get_path()
void CGIHandler::search_cgi(std::vector<std::string> &path){
	int size = path.size();
	for(int i = 0; i < size; i++){
		std::vector<std::string>::iterator it = _cgi_extention.begin();
		while(it != _cgi_extention.end()){
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

void CGIHandler::prepare_cgi_data(HTTPRequest::RequestMessage *_http_request_message, HTTPResponse::SpecifiedConfig &_config, int socket_fd){
	_cgi_extention = _config.get_extention_list();
	_socket_fd = socket_fd;
	std::vector<std::string> path = _http_request_message->get_uri().get_path();
	search_cgi(path);
	if(_search_cgi_extension == false)
		return;	
	_request_message_body = _http_request_message->get_message_body();
	if(pipe(_input_pipe) == Constants::ERROR){
		std::perror("pipe");
		throw(CGIexception());
	}
	if(pipe(_output_pipe) == Constants::ERROR){
		std::perror("pipe");
		throw(CGIexception());
	}
	set_argument(_cgi_name);//TODO replace by the actual path, currently I am only use the predefined path
	struct stat buffer;
	std::string relative_path = "cgi-bin/" + _cgi_name;
	if(stat(relative_path.c_str(), &buffer) != 0)
		_search_cgi_extension = false;
	if(!_search_cgi_extension)
		return;
	parse_meta_variables(_http_request_message, _config);//TODO replace by input from http request get_message_body
	set_envp();
}

void CGIHandler::execute_cgi(int kq)
{
	struct kevent kev;
	EV_SET(&kev, _output_pipe[0], EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(kq, &kev, 1, NULL, 0, NULL)<0){//TODO kevent fail here? @debug
        fprintf(stderr,"kevent failed.");
		std::cout << "this internal error caused by kevent fail\n";
    	return;
	}
	pid_t pid = fork();
	if(pid < 0){
		perror("fork failure");
		throw(CGIexception());
	}
	else if(pid == 0){
		if(dup2(_input_pipe[0], 0) < 0){
			perror("dup 1 failure");
			throw(CGIexception());
		}
		if(dup2(_output_pipe[1], 1) < 0){
			perror("dup 2 failure");
			throw(CGIexception());
		}
		close(_output_pipe[0]);
		close(_input_pipe[1]);
		if(execve(_argument[0], _argument, _envp) == Constants::ERROR){
			perror("execution error");//script is garanteed to be found
			return;
		}
	}
	else{
		pid_t status_var;
		waitpid(pid, &status_var, WNOHANG);
		close(_input_pipe[0]);
	}
}

int CGIHandler::get_read_fd() const{
	return _output_pipe[0];
}

int CGIHandler::get_write_fd() const{
	return _input_pipe[1];
}

void CGIHandler::set_response_message_body(std::string str){
	_response = str;
}

std::string CGIHandler::get_response_message_body(){
	return _response;
}

std::string CGIHandler::get_request_message_body(){
	return _request_message_body;
}

bool CGIHandler::get_search_cgi_extention_result() const{
	return _search_cgi_extension;
}

int CGIHandler::get_socket_fd() const{
	return _socket_fd;
}



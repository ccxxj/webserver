#include "ResponseHandler.hpp"
#include "../Utility/Utility.hpp"
#include "../Constants.hpp"

#include <sstream> // for converting int to string
#include <fstream>  // for ofstream
#include <string.h> //for strerror

#include <sys/event.h>//kqueue

size_t loop_redirection = 0;

namespace HTTPResponse {
	ResponseHandler::ResponseHandler(HTTPRequest::RequestMessage* request_message, ResponseMessage* response_message)
	: _http_request_message(request_message)
	, _http_response_message(response_message)
	{
	}

	ResponseHandler::ResponseHandler(const ResponseHandler& other) {
 		*this = other;
	}

	const ResponseHandler &ResponseHandler::operator=(const ResponseHandler  &other) {
		_http_request_message = other._http_request_message;
		_http_response_message = other._http_response_message;
		_config = other._config;
		_file = other._file;
        return *this;
    }

	ResponseHandler::~ResponseHandler(){}

	bool ResponseHandler::create_http_response(CGI::CGIHandler &cgi_handler, int socket_fd) {
		_file.set_path(_config.get_root(), _http_request_message->get_uri().get_path());
		//log request info
		Utility::logger(request_info(), YELLOW);
		try{
			cgi_handler.prepare_cgi_data(_http_request_message, _config, socket_fd);
			if(cgi_handler.get_search_cgi_extention_result())//if the cgi extention was found in the list, execute cgi and skip the further process
				return false;
		}
		catch(std::exception){
			handle_error(InternalServerError);
			return true;
		}

		// checks before moving on with methods
		if(!_verify_method(_config.get_limit_except())){
			handle_error(MethodNotAllowed);
			return true;
		}

		if (!_check_client_body_size()){
			handle_error(ContentTooLarge);
			return true;
		}

		//redirection: server stops processing, responds with redirected location
		if (!_config.get_return().empty() && loop_redirection < 10) {
			_handle_redirection();
			return true;
		}

		if (loop_redirection == 10) { //redirection is limited to 10 times
			loop_redirection = 0;
		}

		//HTTP method handling
		_handle_methods();
		return true;
	}

	void ResponseHandler::_handle_redirection()
	{
		loop_redirection++;

		//get the redirection information
			//the return directive applies only inside the topmost context it’s defined in
			//so, it's first return directive saved in specified config
		std::map<int, std::string>::const_iterator it = _config.get_return().begin();
		_http_response_message->set_status_code(Utility::to_string(it->first));
		_http_response_message->set_reason_phrase(HTTPResponse::get_reason_phrase(static_cast<HTTPResponse::StatusCode>(it->first)));

		//Save the redirected URL
		_http_response_message->set_header_element("Location", it->second);

		// generate redirection page
		_http_response_message->set_header_element("Last-Modified", Utility::get_formatted_date()); //as it has newly created below
		_http_response_message->set_header_element("Content-Type", "text/html; charset=utf-8");
		_http_response_message->set_message_body(std::string("<html>\r\n<center><h1>")
								+ _http_response_message->get_status_code() + "</h1><center>"
								+ "</center><h2>" + _http_response_message->get_reason_phrase() + "</h2></center>"
								+ "<hr><center> HungerWeb 1.0 </center>\r\n"
								+ "</html>\r\n");

		_build_final_response();
	}

	void ResponseHandler::_handle_methods(void) {
		if (_http_request_message->get_method() == "DELETE")
			_delete_file();
		else if (_http_request_message->get_method() == "POST")
			_upload_file();
		else //GET || HEAD
			_serve_file();
	}

	void ResponseHandler::_serve_file(void) { //GET will retrieve a resource
		if (!_file.exists())
			return handle_error(NotFound);

		if (_file.is_directory()) {
			if (_file.find_index_page(_config.get_index_page())) //automatically looks for an index page
				return(_serve_found_file(_file.get_path() + "/" + _file.get_index_page()));
			else { // directory listing
				if (_config.get_autoindex() == OFF)
					return (handle_error(Forbidden));
				else
					return (_serve_directory());
			}
		}

		if (!_file.is_directory()) { // means its a file
			_serve_found_file(_file.get_path());
		}
	}

	void ResponseHandler::_serve_directory(void) {
		//list the directory into response body
		_http_response_message->set_message_body(_file.list_directory());
		if (_http_response_message->get_message_body().empty())
			return (handle_error(InternalServerError));

		//set necessary headers
		_http_response_message->set_header_element("Content-Type", "text/html; charset=utf-8");
		_http_response_message->set_header_element("Last-Modified", _file.last_modified_info());
		_http_response_message->set_status_code("200");
		_http_response_message->set_reason_phrase("OK");
		_build_final_response();
	}

	void ResponseHandler::_serve_found_file(const std::string &str) {
		_http_response_message->set_message_body(_file.get_content(str));
		if (_http_response_message->get_message_body() == "Forbidden")
			return (handle_error(Forbidden));

		//set necessary headers
		if (_file.get_mime_type(str) == "text/html")
			_http_response_message->set_header_element("Content-Type",  "text/html; charset=utf-8");
		else
			_http_response_message->set_header_element("Content-Type", _file.get_mime_type(str));
		_http_response_message->set_header_element("Last-Modified", _file.last_modified_info(str));
		_http_response_message->set_status_code("200");
		_http_response_message->set_reason_phrase("OK");
		_build_final_response();
	}

	void ResponseHandler::_upload_file(void) { //POST will upload a new resource
		//if there is nothing to upload in request body
		if (_http_request_message->get_message_body().empty())
			return handle_error(BadRequest);
		//URI will only hold directory info and should not point to an existing file
		if (!_file.exists())
			return handle_error(NotFound);
		if (!_file.is_directory()) //means it's a file and it already exists
			return handle_error(Conflict);
		//else target resource is a directory and server creates a file in the upload_dir from config
		//get the upload_dir from config and create it
		if (!_file.create_dir(_file.get_path() + "/"  + _config.get_upload_dir()))
			return handle_error(InternalServerError);

		//extract file name from content-disposition or create randomly named files
		std::string path_and_name;
		if(_http_request_message->has_header_field("CONTENT_DISPOSITION"))
			path_and_name = _file.get_path() + "/"  + _config.get_upload_dir() + "/" + _file.extract_file_name(_http_request_message->get_header_value("CONTENT_DISPOSITION"));
		else if (_http_request_message->has_header_field("CONTENT_TYPE")) {
			path_and_name = _file.get_path() + "/"  + _config.get_upload_dir() + "/" +
			_file.random_name_creator(_file.get_path() + "/"  + _config.get_upload_dir()) +
			 "." + _file.get_extension(_http_request_message->get_header_value("CONTENT_TYPE"));
		}

		if(_http_request_message->has_header_field("CONTENT_TYPE") && _file.get_extension(_http_request_message->get_header_value("CONTENT_TYPE")) == "NotSupported")
			return handle_error(UnsupportedMediaType);

		//create the new resource with the path and put request body inside
		std::ofstream file_stream(path_and_name.c_str());
		if (!file_stream.is_open()) {
			// Utility::logger("DEBUG file_path: " + path_and_name, RED);
			// Utility::logger("DEBUG is_open: " + std::string(strerror(errno)), RED);
			return handle_error(InternalServerError);
		}

		file_stream << _http_request_message->get_message_body();

		// set up response for uploading
		_http_response_message->set_message_body("<h1><center> Successfully created file! </center></h1>");
		_http_response_message->set_header_element("Content-Type", "text/html; charset=utf-8");
		_http_response_message->set_status_code("201");
		_http_response_message->set_reason_phrase("Created");
		_http_response_message->set_header_element("Location", path_and_name);
		_build_final_response();
	}

	void ResponseHandler::_delete_file(void) { //DELETE will delete a resource
		//get file data check for errors with stat
		if (!_file.exists())
			return handle_error(NotFound);

		if (!_file.is_regular()) //allowing normal files to be removed
			return handle_error(Forbidden);

		//remove the file
		if (!_file.un_link(_file.get_path()))
			return (handle_error(InternalServerError));

		_http_response_message->set_status_code("200");
		_http_response_message->set_reason_phrase("OK");
		_http_response_message->set_header_element("Content-Type", "text/html; charset=utf-8");
		_http_response_message->set_message_body("<html>\r\n<body><center>\r\n<h1>File deleted.\r\n</h1>\r\n</center></body>\r\n</html>");
		_http_response_message->set_header_element("Last-Modified", Utility::get_formatted_date()); //as it has newly created below
		_build_final_response();
	}

	void ResponseHandler::handle_error(HTTPResponse::StatusCode code)
	{
		_http_response_message->set_status_code(Utility::to_string(static_cast<int>(code)));
		_http_response_message->set_reason_phrase(HTTPResponse::get_reason_phrase(code));

		if (code == MethodNotAllowed)
			_http_response_message->set_header_element("Allow", _config.get_methods_line());

		//handle custom error pages
		if (!_config.get_error_page().empty()) {
			std::map<int, std::string>::const_iterator it = _config.get_error_page().find(static_cast<int>(code));
			if (it != _config.get_error_page().end()) {
				return _serve_custom_error_page(it->second);
			}
		}

		// generate error page
		_http_response_message->set_header_element("Last-Modified", Utility::get_formatted_date()); //as it has newly created below
		_http_response_message->set_header_element("Content-Type", "text/html; charset=utf-8");
		_http_response_message->set_message_body(std::string("<html>\r\n<center><h1>")
								+ _http_response_message->get_status_code() + "</h1><center>"
								+ "</center><h2>" + _http_response_message->get_reason_phrase() + "</h2></center>"
								+ "<hr><center> HungerWeb 1.0 </center>\r\n"
								+ "</html>\r\n");

		_build_final_response();
	}

	void ResponseHandler::_serve_custom_error_page(const std::string &str) {
		_file.set_root("www");
		_http_response_message->set_message_body(_file.get_content(_file.get_root() + str));
		if (_http_response_message->get_message_body() == "Forbidden")
			return (handle_error(Forbidden));

		//log error_page redirection
		Utility::logger("Internal redirect [error_page " + str + "] " +
						"[Root " + _file.get_root() + "] " +
						"[Search Path " + _file.get_root() + str + "] "
						, MAGENTA);
		//set necessary headers
		_http_response_message->set_header_element("Content-Type", _file.get_mime_type(str));
		_http_response_message->set_header_element("Last-Modified", _file.last_modified_info(_file.get_root() + str));
		_build_final_response();
	}


	void ResponseHandler::_build_final_response()
	{
		std::string response;
		std::string msg_body = _http_response_message->get_message_body();

		// set any remaining headers
		if(_http_request_message->get_method() != "HEAD")
			_http_response_message->set_header_element("Content-Length", Utility::to_string(msg_body.length()));
		_http_response_message->set_header_element("Date", Utility::get_formatted_date());
		_http_response_message->set_header_element("Server", "HungerWeb/1.0");

		// build status line
		response += _http_response_message->get_HTTP_version() + " ";
		response += _http_response_message->get_status_code() + " ";
		response += _http_response_message->get_reason_phrase() + "\r\n";

		// add all the headers to response. Format is {Header}: {Header value} \r\n
		std::map<std::string, std::string>::const_iterator it;
		for (it = _http_response_message->get_response_headers().begin(); it != _http_response_message->get_response_headers().end(); it++) {
			if (!it->first.empty())
				response += it->first + ": " + it->second;
			response += "\r\n";
		}

		// if body is not empty add it to  response. Format: \r\n {body}
		response += "\r\n";
		if(!msg_body.empty() && _http_request_message->get_method() != "HEAD")
			response += msg_body;

		//final step
		_http_response_message->append_complete_response(response);

		//log
		Utility::logger(response_status(), PURPLE);
	}

	bool ResponseHandler::_verify_method(const std::vector<std::string> methods) {
		if (methods.empty())
			return true;
		const std::string request_method = _http_request_message->get_method();
		for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); it++)
			if (request_method.compare(*it) == 0)
				return true;
		return false;
	}

	bool ResponseHandler::_check_client_body_size() {
		int body_size = _http_request_message->get_message_body().size();
		if (body_size > _config.get_client_max_body_size())
			return false;
		return true;
	}

	void ResponseHandler::set_config_rules(const Config::ServerBlock *virtual_server, const Config::LocationBlock *location) {
		//directive values between levels are generally inherited or replaced, but not added
		//i.e. only if there are no error_page directives defined on the current level, outer level's are inherited
		_config.set_id(virtual_server->get_id());
		if (location && location->get_error_page().size())
			_config.set_error_page_value(location->get_error_page());
		else
			_config.set_error_page_value(virtual_server->get_error_page());
		if (location && !location->get_is_size_default()) //we decided to overwrite irrespective of the relationship between levels
			_config.set_client_max_body_size(location->get_client_max_body_size());
		else
			_config.set_client_max_body_size(virtual_server->get_client_max_body_size());


		_config.set_root_value(virtual_server->get_root()); //if loc has root, this will be overwritten
		_config.set_index_page(virtual_server->get_index_page()); //if loc has index, this will be overwritten
		_config.set_return_value(virtual_server->get_return()); //returns are appended within levels
		_config.set_extention_list(virtual_server->get_extention_list());
		if(location) { //location specific config rules, appends and overwrites
			_config.set_limit_except(location->get_limit_except());
			_config.set_methods_line(location->get_limit_except());
			_config.set_autoindex(location->get_autoindex());
			_config.set_route(location->get_route());
			_config.set_upload_dir(location->get_upload_dir());
			if (!location->get_root().empty())
				_config.set_root_value(location->get_root());
			if (location->get_index_page() != "index.html") //different from the default one
				_config.set_index_page(location->get_index_page());
			_config.set_return_value(location->get_return());
		}
	}

	std::string ResponseHandler::response_status() {
		std::string tmp;

		tmp += "Response ";
		tmp += "[Status " + _http_response_message->get_status_code();
		tmp += " " + _http_response_message->get_reason_phrase() + "]";

		return tmp;
	}

	std::string ResponseHandler::request_info() {
		std::string tmp;

		tmp += "Request  ";
		tmp += "[Method " + _http_request_message->get_method() + "] ";
		tmp += "[Target " + _file.get_target() + "] ";
		tmp += "[Server " + Utility::to_string(_config.get_id()) + "] ";
		tmp += "[Location " + _config.get_route() + "] ";
		tmp += "[Root " + _config.get_root() + "] ";
		tmp += "[Search Path " + _file.get_path() + "] ";

		return tmp;
	}
}

#include "Connection.hpp"
#include "../config/ConfigData.hpp"
#include "../Utility/Utility.hpp"

#include <string>
#include <iostream>
#include <unistd.h>
#include <errno.h>


namespace HTTP {
	Connection::Connection(int connection_socket_fd, Config::ConfigData *config_data, ListenInfo& listen_info, sockaddr_in connection_addr)
		: _socket_fd(connection_socket_fd)
		, _listen_info(listen_info)
		, _is_open(true)
		, logtime_counter()
		, request_handler(new RequestHandler(*this, config_data, _listen_info))
		, my_connection_addr(connection_addr)
		{
			_cgi_write_read_fd[0] = -1;
			_cgi_write_read_fd[1] = -1;
		}

	Connection::~Connection(){
	}

	void Connection::handle_http_request(int kq) {
		request_handler->handle_http_request(kq, _socket_fd);
		if(request_handler->get_search_cgi_extention_result()){
			set_cgi_write_fd(request_handler->get_cgi_write_fd());
			set_cgi_read_fd(request_handler->get_cgi_read_fd());
		}
	}
 
	void Connection::send_response() {
		request_handler->send_response();
	}

	void Connection::set_cgi_write_fd(int i){
		_cgi_write_read_fd[0] = i;
	}

	void Connection::set_cgi_read_fd(int i){
		_cgi_write_read_fd[1] = i;
	}
	
	void Connection::handle_internal_server_error(){
		request_handler->handle_internal_server_error();
	}

	bool Connection::is_connection_open() const {
		return _is_open;
	}

	bool Connection::is_hanging_connection() {
		return logtime_counter.is_bigger_than_time_limit(Constants::NO_ACTIVITY_TIMEOUT);
	}

	int Connection::get_cgi_write_fd() const{
		return _cgi_write_read_fd[0];
	}

	int Connection::get_cgi_read_fd() const{
		return _cgi_write_read_fd[1];
	}	

	std::string Connection::get_request_message_body(){
		return request_handler->get_request_message_body();
	}

	void Connection::send(std::string& buffer, size_t buffer_size) {
		size_t current_buffer_size;
		if (buffer_size < Constants::SEND_BUFFER_SIZE) {
			current_buffer_size = buffer_size;
		}
		else {
			current_buffer_size = Constants::SEND_BUFFER_SIZE;
		}
		ssize_t bytes_sent = ::send(_socket_fd, buffer.c_str(), current_buffer_size, 0);
		if (bytes_sent < 0) {
			Utility::logger("Send failed. errno: " + Utility::to_string(errno), RED);
			this->close();
			return;
		}
		logtime_counter.update_last_activity_logtime();
		if (bytes_sent == 0) {
			return;
		}
		if (buffer_size > (size_t)bytes_sent) { // erasing the part that has been sent if the buffer is bigger than we can handle
			buffer.erase(0, (size_t)bytes_sent);
		}
		else {
			buffer.clear();
			this->close();
		}
	}

	void Connection::close() {
		if (::close(_socket_fd) < 0) {
			Utility::logger("Socket closing failed. errno: "  + Utility::to_string(errno), RED);
		} else {
			// Utility::logger("Socket " +  Utility::to_string(_socket_fd) + " is closed.", PURPLE); // for debug
			_is_open = false;
		}
	}

	size_t Connection::receive(char* buffer, size_t buffer_size) {
		logtime_counter.update_last_activity_logtime();
		return ::recv(_socket_fd, buffer, buffer_size, 0);
	}

	int Connection::get_fd(){
		return _socket_fd;
	}

	HTTPResponse::ResponseMessage &Connection::get_response_message(){
		return request_handler->get_http_response_message();
	}

	void Connection::set_response_true(){
		request_handler->set_response_true();
	}

	void Connection::execute_cgi(int kq){
		request_handler->execute_cgi(kq);
	}
}

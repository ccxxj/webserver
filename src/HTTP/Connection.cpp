#include "Connection.hpp"
#include "../config/ConfigData.hpp"
#include "../Constants.hpp"
#include "../Utility/Utility.hpp"

#include <string>
#include <iostream>
#include <unistd.h>
#include <errno.h>


namespace HTTP {
	Connection::Connection(int connection_socket_fd, Config::ConfigData *config_data, ListenInfo& listen_info, sockaddr_in connection_addr)
		// Connection::Connection(int connection_socket_fd, int server_listening_sockfd, sockaddr_in& connection_addr, int connection_addr_len)
		: _socket_fd(connection_socket_fd)
		, _listen_info(listen_info)
		, _is_open(true)
		, request_handler(new RequestHandler(*this, config_data, _listen_info))
		, my_connection_addr(connection_addr)
	// , _listening_socket_fd(server_listening_sockfd)
	//	, _client_addr(connection_addr)
	// , _client_addr_len(connection_addr_len)
		{}

	Connection::~Connection(){
	}

	void Connection::handle_http_request() {
		std::cout << "C: " << my_connection_addr.sin_addr.s_addr << std::endl;
		request_handler->handle_http_request();
	}

	bool Connection::is_connection_open() const {
		return _is_open;
	}

	void Connection::send(const void* buffer, size_t buffer_size) {
		size_t ret = ::send(_socket_fd, buffer, buffer_size, 0);
		if ( ret < 0) {
			Utility::logger("Send failed. errno: " + Utility::to_string(errno), RED);
			this->close();
		}
		if (ret == buffer_size)
			this->close();
	}

	void Connection::close() {
		if (::close(_socket_fd) < 0) {
			Utility::logger("Socket closing failed. errno: "  + Utility::to_string(errno), RED);
		} else {
			Utility::logger("Socket " +  Utility::to_string(_socket_fd) + " is closed.", PURPLE); // for debug
			_is_open = false;
		}
	}

	size_t Connection::receive(char* buffer, size_t buffer_size) {
		return ::recv(_socket_fd, buffer, buffer_size, 0);
	}
}

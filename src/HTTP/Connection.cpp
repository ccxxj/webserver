#include "Connection.hpp"
#include "../config/ConfigData.hpp"

#include <string>
#include <iostream>
#include <unistd.h>
#include <errno.h>


namespace HTTP {
	Connection::Connection(int connection_socket_fd, Config::ConfigData *config_data, ListenInfo& listen_info)
		// Connection::Connection(int connection_socket_fd, int server_listening_sockfd, sockaddr_in& connection_addr, int connection_addr_len)
		: _socket_fd(connection_socket_fd)
		, _listen_info(listen_info)
		, request_handler(new RequestHandler(*this, config_data, listen_info))
	// , _listening_socket_fd(server_listening_sockfd)
	//	, _client_addr(connection_addr)
	// , _client_addr_len(connection_addr_len)
		{}

	Connection::~Connection(){
	}

	void Connection::handle_http_request() {
		// TODO this conversion failed. Trying another solution atm.
		// uint32_t port = ntohs(_client_addr.sin_port);
		// // uint32_t addr = ntohl(_client_addr.sin_addr);
		std::cout << "\n\033[31mIP addr: " << _listen_info.ip << std::endl;
		std::cout << "Port: " << _listen_info.port << "\033[0m\n" << std::endl;
		request_handler->handle_http_request();
	}

	void Connection::send(const void* buffer, size_t buffer_size) {
		if (::send(_socket_fd, buffer, buffer_size, 0) < 0) {
			std::cout << "Send failed. errno: " << errno << std::endl;
			this->close();
		}
	}

	void Connection::close() {
		if (::close(_socket_fd) < 0) {
			std::cout << "Socket closing failed. errno: " << errno << std::endl;
		} else {
			std::cout << "Socket " << _socket_fd << " is closed." << std::endl; // for debug
		}
	}

	size_t Connection::receive(char* buffer, size_t buffer_size) {
		return ::recv(_socket_fd, buffer, buffer_size, 0);
	}
}

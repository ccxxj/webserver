#include "Connection.hpp"

#include <string>
#include <iostream>
#include <unistd.h>
#include <errno.h>

namespace HTTP {
	Connection::Connection() {}
	Connection::Connection(int connection_socket_fd) 
	// Connection::Connection(int connection_socket_fd, int server_listening_sockfd, sockaddr_in& connection_addr, int connection_addr_len) 
		: _socket_fd(connection_socket_fd)
		// , _listening_socket_fd(server_listening_sockfd)
		// , _client_addr(connection_addr)
		// , _client_addr_len(connection_addr_len) 
		{}

	Connection::~Connection(){}


	void Connection::send(const void* buffer, size_t buffer_size) {
		if (::send(_socket_fd, buffer, buffer_size, 0) < 0) {
			std::cout << "Send failed. errno: " << errno << std::endl;
			this->close();
		}
	}

	void Connection::close() {
		if (::close(_socket_fd) < 0) {
			std::cout << "Socket closing failed. errno: " << errno << std::endl;
		}
	}

	size_t Connection::recv(char* buffer, size_t buffer_size) {
		return ::recv(_socket_fd, buffer, buffer_size, 0);
	}
}

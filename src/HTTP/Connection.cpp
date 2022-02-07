#include "Connection.hpp"


namespace HTTP {
	Connection::Connection() {}
	Connection::Connection(int connection_socket_fd, int server_listening_sockfd, sockaddr_in& connection_addr, int connection_addr_len) 
		: _socket_fd(connection_socket_fd)
		, _listening_socket_fd(server_listening_sockfd)
		, _client_addr(connection_addr)
		, _client_addr_len(connection_addr_len) {}

	Connection::~Connection(){}

	int Connection::get_socket_fd() {
		return _socket_fd;
	}


}
#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <netinet/in.h> // for sockaddr_in struct

namespace HTTP {
	class Connection
	{
	private:
		int _socket_fd;
		// int _listening_socket_fd;
		// sockaddr_in _client_addr;
		// int _client_addr_len;

		Connection();
	public:
		Connection(int connection_socket_fd);
		// Connection(int connection_socket_fd, int server_listening_sockfd, sockaddr_in& connection_addr, int connection_addr_len);
		~Connection();

		size_t recv(char *buffer, size_t buffer_size);
		void send(const void *buffer, size_t buffer_size);
		void close();
	};
}

#endif

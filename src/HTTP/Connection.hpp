#pragma once

#include <netinet/in.h> // for sockaddr_in struct
#include <memory> // for auto_ptr

#include "RequestHandler.hpp"
#include "RequestHandlerDelegate.hpp"

namespace HTTP {
	class Connection : public RequestHandlerDelegate
	{
	private:
		int _socket_fd;
		std::auto_ptr<RequestHandler> request_handler;

		// int _listening_socket_fd;
		// sockaddr_in _client_addr;
		// int _client_addr_len;
	public:
		Connection(int connection_socket_fd);
		// Connection(int connection_socket_fd, int server_listening_sockfd, sockaddr_in& connection_addr, int connection_addr_len);
		~Connection();

		void handle_http_request();
		virtual size_t receive(char *buffer, size_t buffer_size);
		virtual void send(const void *buffer, size_t buffer_size);
		virtual void close();
	};
}

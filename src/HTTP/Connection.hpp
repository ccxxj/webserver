#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <netinet/in.h> // for sockaddr_in struct

namespace HTTP {
	class Connection
	{
		friend class Server;

	private:
		int _socket_fd;
		int _listening_socket_fd;
		sockaddr_in _client_addr;
		int _client_addr_len;
		Connection();
	public:
		Connection(int server_listening_sockfd);
		~Connection();

		void handle_http_request();
	};
}

#endif

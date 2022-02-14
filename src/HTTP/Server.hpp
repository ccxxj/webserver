#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <map>
#include "Connection.hpp"

namespace HTTP {

	class Server{
	private:
		void _handle_events();
		void _setup_listening_sockets();
		bool _is_in_listen_sockfd_list(int fd);

		void _remove_closed_connection(int fd);

		std::vector<int> _listen_ports;
		std::vector<int> _listening_sockfds;
		std::map<int, Connection> _connections;

	public:
		Server();
		~Server();
		void run();
	};
	void signal_handler(int signal);
}

#endif

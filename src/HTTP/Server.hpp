#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <cstdlib>
#include <cstring>
#include "Connection.hpp"
#include "../config/ConfigData.hpp"

namespace HTTP {

	class Server{
	private:
		Config::ConfigData *config_data;
		void _handle_events();
		void _setup_listening_sockets();
		bool _is_in_listen_sockfd_list(int fd);

		void _remove_closed_connection(int fd);

		std::vector<int> _listen_ports;
		std::vector<int> _listening_sockfds;
		std::vector<Connection> _connections;

	public:
		Server(Config::ConfigData *config_data);
		~Server();
		void run();
	};
}

#endif

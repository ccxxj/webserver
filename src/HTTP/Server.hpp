#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include "Connection.hpp"
#include "../config/ConfigData.hpp"
#include "ServerStructs.hpp"

namespace HTTP {

	class Server{

	private:
		Config::ConfigData *config_data;

		void _handle_events();
		void _setup_listening_sockets();
		bool _is_in_listen_sockfd_list(int fd);
		void _setup_listening_ports();

		void _remove_disconnected_client(int fd);
		void _remove_connection_closed_by_server(int sock_kqueue);
		void _destroy_connection(std::map<int, Connection*>::iterator iterator);

		std::vector<int> _listen_ports;
		std::vector<int> _listening_sockfds;
		std::map<int, Connection*> _connections;
		std::map<int, ListenInfo> _running_servers;
		std::set<int> _bound_ports;

	public:
		Server(Config::ConfigData *config_data);
		~Server();
		void run();
	};
}

#endif

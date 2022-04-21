#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include "Connection.hpp"
#include "../config/ConfigData.hpp"
#include "ServerStructs.hpp"
#include "../CGI/CGIHandler.hpp"

namespace HTTP {

	class Server{

	private:
		Config::ConfigData* config_data;
		Utility::LogTimeCounter _logtime_checker;
		// CGIHandler	_cgi_handler;

		void _handle_events();
		void _setup_listening_sockets();
		bool _is_in_listen_sockfd_list(int fd);
		void _setup_listening_ports();
		void _handle_disconnected_client(int current_event_fd);
		void _remove_disconnected_client(int fd);
		void _close_hanging_connections(int sock_kqueue);
		void _accept_new_connection(int current_event_fd, int sock_kqueue);
		void _handle_read_event(int current_event_fd, int sock_kqueue);
		void _handle_write_event(int current_event_fd, int sock_kqueue);
		void _handle_read_end_of_pipe();
		void _handle_write_end_of_pipe(int sock_kqueue);
		void _delete_events(int sock_kqueue, int identifier);
		std::map<int, Connection*>::iterator _destroy_connection(std::map<int, Connection *>::iterator iterator);

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

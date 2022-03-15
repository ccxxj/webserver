#include "Server.hpp"

#include <sys/socket.h> // for socket
#include <sys/errno.h>
#include <unistd.h> // for close
#include <iostream>
#include <string>
#include <string.h> // strerror TODO: remove
#include  <cstdlib> // for exit
#include <cstdio> // for perror
#include <fcntl.h> // for fcntl
#include <sys/time.h> // for timeout
#ifdef _LINUX
	#include "/usr/include/kqueue/sys/event.h" //linux kqueue
#else
	#include <sys/event.h> // for kqueue and kevent
#endif

#include "RequestHandler.hpp"
#include "../globals.hpp"

namespace HTTP {

	Server::Server(Config::ConfigData *config_data): config_data(config_data)
	{
	}

	Server::~Server(){
		std::vector<int>::iterator it = _listening_sockfds.begin();
		for (; it != _listening_sockfds.end(); ++it) {
			close(*it); //closing listening sockets
		}
	}

	void Server::_setup_listening_sockets() {
		for(size_t i = 0; i < _listen_ports.size(); i++) {
			_listening_sockfds.push_back(socket(AF_INET, SOCK_STREAM, 0));
			if (_listening_sockfds[i] < 0) {
				std::cout << "Socket failed. errno: " << errno << std::endl;
				std::exit(EXIT_FAILURE);
			}
			// When retrieving a socket option, or setting it, you specify the option name as well as the level. When level = SOL_SOCKET, the item will be searched for in the socket itself.
			int value = 1;
			// SO_REUSEADDR Reports whether the rules used in validating addresses supplied to bind() should allow reuse of local addresses,
			// if this is supported by the protocol. More explanation in the docs/resoures/#Sockets
			if (setsockopt(_listening_sockfds[i], SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) < 0) {
				std::cout << "Setting SO_REUSEADDR failed. errno: " << errno << std::endl;
				std::exit(EXIT_FAILURE);
			}
			// TODO: adding other socket options like TCP_DEFER_ACCEPT?
			sockaddr_in sockaddr;
			sockaddr.sin_family = AF_INET;
			sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);// this is the address for this socket. The special adress for this is 0.0.0.0, defined by symbolic constant INADDR_ANY
			sockaddr.sin_port = htons(_listen_ports[i]);//htons is necessary to convert a number to network byte order
			if(bind(_listening_sockfds[i], (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) { //int bind(int sockfd, const sockaddr *addr, socklen_t addrlen); return -1 in case of error, return 0 in case of success;
				std::cout << "Failed to bind to port " << _listen_ports[i] << " errno: " << errno << std::endl;
				std::exit(EXIT_FAILURE);
			}
			if (listen(_listening_sockfds[i], 10) < 0) {
				std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
				std::exit(EXIT_FAILURE);
			}
			if (fcntl(_listening_sockfds[i], F_SETFL, O_NONBLOCK) == ERROR) {
				std::perror("fcntl error");
			}
			ListenInfo each_listen("0.0.0.0", _listen_ports[i]); //this struct will hold both ip and port info of running servers
			_running_servers[_listening_sockfds[i]] = each_listen;
			std::cout << "***************The server is listening on port: " << _listen_ports[i] <<"***************" << std::endl;
		}
	}

	// TODO: is the size of listening_sockfd changing? if not create an attribute
	bool Server::_is_in_listen_sockfd_list(int fd) {
		for(size_t i = 0; i < _listening_sockfds.size(); i++) {
			if(fd == _listening_sockfds[i]) {
				return true;
			}
		}
		return false;
	}

	void Server::_remove_closed_connection(int fd) {
		std::map<int, Connection*>::iterator iter = _connections.begin();
		while (iter != _connections.end()) {
			if (iter->first == fd) {
				_connections.erase(iter);
				break;
			}
			iter++;
		}
	}

	void Server::_handle_events() {
		int sock_kqueue = kqueue(); //creates a new kernel event queue and returns a descriptor.
		if (sock_kqueue < 0) {
			std::cout << "Error creating kqueue. errno: " << errno << std::endl;
			std::exit(EXIT_FAILURE);
		}
		struct kevent kev[10], event_fds[10]; // kernel event
		for(size_t i = 0; i < _listen_ports.size(); i++) {
			EV_SET(kev, _listening_sockfds[i], EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0); // is a macro which is provided for ease of initializing a kevent structure.
			if (kevent(sock_kqueue, kev, 1, NULL, 0, NULL) < 0) {
				std::perror("kevent");
				std::exit(1);
			}
		}
		while (true) {
			struct timespec timeout;
			timeout.tv_sec = 30;
			timeout.tv_nsec = 0;
			int new_events = kevent(sock_kqueue, NULL, 0, event_fds, 1, &timeout); //look out for events and register to event list; one event per time
			if(new_events == -1) {
				std::perror("kevent");
				std::exit(1);
			}
			if(new_events == 0) {
				std::map<int, Connection*>::iterator iter = _connections.begin();
				while (iter != _connections.end()) {
					if (iter->second->is_connection_open()) {
						close(iter->first);
					}
					_connections.erase(iter);
					if (_connections.size() == 0) {
						break;
					}
					iter++;
				}
			}
			for(int i = 0; i < new_events; i++) {
				int current_event_fd = event_fds[i].ident;
				if (event_fds[i].flags & EV_ERROR) {
					std::cout << "Event error: %s", strerror(event_fds[i].data);
					std::exit(EXIT_FAILURE);
				}
				else if (event_fds[i].flags & EV_EOF) {
					std::cout << "The client has disconnected." << std::endl;
					close(current_event_fd);
					_remove_closed_connection(current_event_fd);
					std::cout << "FD " << current_event_fd << " is closed and removed from _connections." << std::endl;
				}
				else if(_is_in_listen_sockfd_list(current_event_fd)) {

					sockaddr_in connection_addr;
					int connection_addr_len = sizeof(connection_addr);
					int connection_socket_fd = accept(current_event_fd, (struct sockaddr *)&connection_addr, (socklen_t *)&connection_addr_len);
					if (connection_socket_fd == -1)
					{
						std::perror("accept socket error");
					}
					if (fcntl(connection_socket_fd, F_SETFL, O_NONBLOCK) == ERROR) {

						std::perror("fcntl error");
					}
					//TODO:: check if these are needed Connection connection(connection_socket_fd, current_event_fd, connection_addr, connection_addr_len);
					std::cout << "current con fd: " << current_event_fd << std::endl;
					std::cout << "IP addr: " << _running_servers[current_event_fd].ip << std::endl;
					std::cout << "Port: " << _running_servers[current_event_fd].port << std::endl;
					Connection* connection_ptr = new Connection(connection_socket_fd, config_data, _running_servers[current_event_fd]);
					_connections.insert(std::make_pair(connection_socket_fd, connection_ptr)); // TODO: either make sure you're deleting connection or implement a smart_pointer class
					EV_SET(kev, connection_socket_fd, EVFILT_READ, EV_ADD, 0, 0, NULL); //put socket connection into the filter
					if (kevent(sock_kqueue, kev, 1, NULL, 0, NULL) < 0) {
						std::perror("kevent error");
					}
				}
				else if (event_fds[i].filter & EVFILT_READ) {
					std::map<int, Connection*>::iterator connection_iter = _connections.find(current_event_fd);
					if (connection_iter != _connections.end()) { // handling request by the corresponding connectio
						(connection_iter->second)->handle_http_request();
						break;
					}
				}
			}
		}
	}

	void Server::run() {
		const std::vector<Config::ServerBlock> servers = config_data->get_servers();
		for (size_t i = 0; i < servers.size(); i++)
		{
			std::set<std::string> listen_set = servers[i].get_listen();
			//TODO [::]:1000's atoi result is 0 since the string starts with non-numerical number.
			for (std::set<std::string>::iterator i = listen_set.begin(); i != listen_set.end(); i++) {
				int port = std::atoi((*i).c_str());
				if (std::find(_listen_ports.begin(), _listen_ports.end(), port) == _listen_ports.end()) //does not push duplicate ports
					_listen_ports.push_back(port); 
			}		
		}
		_setup_listening_sockets();
		_handle_events();
	}
}

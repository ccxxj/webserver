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
				std::cout << "Failed to bind to port " << _listen_ports[i] << "errno: " << errno << std::endl;
				std::exit(EXIT_FAILURE);
			}
			if (listen(_listening_sockfds[i], 10) < 0) {
				std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
				std::exit(EXIT_FAILURE);
			}
			if (fcntl(_listening_sockfds[i], F_SETFL, O_NONBLOCK) == ERROR) {
				std::perror("fcntl error");
			}
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
		std::map<int, Connection>::iterator iter = _connections.begin();
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
			//TODO: addding timeout as the last parameter?
			int new_events = kevent(sock_kqueue, NULL, 0, event_fds, 1, NULL);//look out for events and register to event list; one event per time
			if(new_events == -1) {
				std::perror("kevent");
				std::exit(1);
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
					Connection connection(connection_socket_fd);
					_connections.insert(std::make_pair(connection_socket_fd, connection));
					EV_SET(kev, connection_socket_fd, EVFILT_READ, EV_ADD, 0, 0, NULL); //put socket connection into the filter
					if (kevent(sock_kqueue, kev, 1, NULL, 0, NULL) < 0) {
						std::perror("kevent error");
					}
				}
				else if (event_fds[i].filter & EVFILT_READ) {
					std::map<int, Connection>::iterator connection_iter = _connections.find(current_event_fd);
					if (connection_iter != _connections.end()) {
							RequestHandler request_handler(connection_iter->second);
							request_handler.handle_http_request();
							break;
					}
				}
			}
		}
	}

	void Server::run() {
		std::vector<Config::ServerBlock> servers = config_data->get_servers();
		//TODO implemented on the idea that each server block will have on port
		for (size_t i = 0; i < servers.size(); i++)
			_listen_ports.push_back(std::atoi(servers[i].get_listen()[0].c_str()));
		_setup_listening_sockets();
		_handle_events();
	}
}

#include "Server.hpp"

#include <sys/socket.h> // for socket
#include <sys/event.h> // for kqueue and kevent
#include <sys/errno.h>
#include <unistd.h> // for close
#include <iostream>
#include <string>

namespace HTTP {

	Server::Server(){}

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
				exit(EXIT_FAILURE);
			}
			// When retrieving a socket option, or setting it, you specify the option name as well as the level. When level = SOL_SOCKET, the item will be searched for in the socket itself.
			int value = 1;
			// SO_REUSEADDR Reports whether the rules used in validating addresses supplied to bind() should allow reuse of local addresses,
			// if this is supported by the protocol. More explanation in the docs/resoures/#Sockets
			if (setsockopt(_listening_sockfds[i], SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) < 0) {
				std::cout << "Setting SO_REUSEADDR failed. errno: " << errno << std::endl;
				exit(EXIT_FAILURE);
			}
			// TODO: adding other socket options like TCP_DEFER_ACCEPT?

			sockaddr_in sockaddr;
			sockaddr.sin_family = AF_INET;
			sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);// this is the address for this socket. The special adress for this is 0.0.0.0, defined by symbolic constant INADDR_ANY
			sockaddr.sin_port = htons(_listen_ports[i]);//htons is necessary to convert a number to network byte order
			if(bind(_listening_sockfds[i], (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) { //int bind(int sockfd, const sockaddr *addr, socklen_t addrlen); return -1 in case of error, return 0 in case of success;
				std::cout << "Failed to bind to port " << _listen_ports[i] << "errno: " << errno << std::endl;
				exit(EXIT_FAILURE);
			}
			if (listen(_listening_sockfds[i], 10) < 0) {
				std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
				exit(EXIT_FAILURE);
			}
			std::cout << "***************The server is listening on port: " << _listen_ports[i] <<"***************" << std::endl;
		}
	}

	//TODO: is the size of listening_sockfd changing? if not create an attribute
	bool Server::_is_in_listen_sockfd_list(int fd) {
		for(size_t i = 0; i < _listening_sockfds.size(); i++) {
			if(fd == _listening_sockfds[i]) {
				return true;
			}
		}
		return false;
	}

	void Server::_handle_events() {
		int sock_kqueue = kqueue(); //creates a new kernel event queue and returns a descriptor.
		if (sock_kqueue < 0) {
			std::cout << "Error creating kqueue. errno: " << errno << std::endl;
			exit(EXIT_FAILURE);
		}
		struct kevent kev[10], event[10]; // kernel event
		for(size_t i = 0; i < _listen_ports.size(); i++) {
			EV_SET(kev, _listening_sockfds[i], EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0); // is a macro which is provided for ease of initializing a kevent structure. 
			if (kevent(sock_kqueue, kev, 1, NULL, 0, NULL) < 0) {
				perror("kevent");
				exit(1);
			}
		}
		while (true) {
			//TODO: addding timeout as the last parameter?
			int new_events = kevent(sock_kqueue, NULL, 0, event, 1, NULL);//look out for events and register to event list; one event per time
			if(new_events == -1) {
				perror("kevent");
				exit(1);
			}
			for(int i = 0; i < new_events; i++) {
				int event_fd = event[i].ident;
				if (event[i].flags & EV_ERROR) {
					std::cout << "Event error: %s", strerror(event[i].data);
					exit(EXIT_FAILURE);
				}
				else if (event[i].flags & EV_EOF) {
					std::cout << "The client has disconnected." << std::endl;
					close(event_fd);
					std::cout << "FD " << event_fd << " is closed." << std::endl;
				}
				else if(_is_in_listen_sockfd_list(event_fd)) {
					Connection connection(event_fd);
					connection._socket_fd = accept(connection._listening_socket_fd, (struct sockaddr *)&(connection._client_addr), (socklen_t *)&(connection._client_addr_len));
					if (connection._socket_fd == -1)
					{
						perror("accept socket error");
					}
					_connections.push_back(connection);
					EV_SET(kev, connection._socket_fd, EVFILT_READ, EV_ADD, 0, 0, NULL); //put socket connection into the filter
					if (kevent(sock_kqueue, kev, 1, NULL, 0, NULL) < 0) {
						perror("kevent error");
					}
				}
				else if (event[i].filter & EVFILT_READ) {
					Connection active_connection;
					for (size_t j = 0; j < _connections.size(); ++j)
					{
						if (_connections[j]._socket_fd == event_fd) {
							active_connection = _connections[j];
							active_connection.handle_http_request(); //TODO: to be replaced by a proper handler
						}
					}
					std::string response = "I do exist!\n";
					send(active_connection._socket_fd, response.c_str(), response.size(), 0);
				}
			}
		}
	}

	void Server::run() {
		//TODO: hardcoded values will be replaced after config parsing
		_listen_ports.push_back(8080);
		_listen_ports.push_back(1000);
		_listen_ports.push_back(20);
		_setup_listening_sockets();
		_handle_events();
	}
}
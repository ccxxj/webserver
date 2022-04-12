#include "Server.hpp"

#include <sys/socket.h> // for socket
#include <sys/errno.h>
#include <unistd.h> // for close
#include <iostream>
#include <string>
#include <algorithm>
#include <string.h> // strerror TODO: remove
#include  <cstdlib> // for exit
#include <cstdio> // for perror
#include <fcntl.h> // for fcntl
#include <sys/time.h> // for timeout
#include <sys/stat.h> // for fstat
#ifdef _LINUX
	#include "/usr/include/kqueue/sys/event.h" //linux kqueue
#else
	#include <sys/event.h> // for kqueue and kevent
#endif

#include "RequestHandler.hpp"
#include "../Utility/Utility.hpp"


namespace HTTP {

	Server::Server(Config::ConfigData *config_data)
	: config_data(config_data)
	, _logtime_checker()
	{}

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
				Utility::logger("Socket failed. errno: "  + Utility::to_string(errno), RED);
				std::exit(EXIT_FAILURE);
			}
			// When retrieving a socket option, or setting it, you specify the option name as well as the level. When level = SOL_SOCKET, the item will be searched for in the socket itself.
			int value = 1;
			// SO_REUSEADDR Reports whether the rules used in validating addresses supplied to bind() should allow reuse of local addresses,
			// if this is supported by the protocol. More explanation in the docs/resoures/#Sockets
			if (setsockopt(_listening_sockfds[i], SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) < 0) {
				Utility::logger("Setting SO_REUSEADDR failed. errno: " + Utility::to_string(errno), RED);
				std::exit(EXIT_FAILURE);
			}
			sockaddr_in sockaddr;
			sockaddr.sin_family = AF_INET;
			sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);// this is the address for this socket. The special adress for this is 0.0.0.0, defined by symbolic constant INADDR_ANY
			sockaddr.sin_port = htons(_listen_ports[i]);//htons is necessary to convert a number to network byte order
			if(bind(_listening_sockfds[i], (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) { //int bind(int sockfd, const sockaddr *addr, socklen_t addrlen); return -1 in case of error, return 0 in case of success;
				Utility::logger("Failed to bind to port " +  Utility::to_string(_listen_ports[i]) + " errno: " +  Utility::to_string(errno), RED);
				std::exit(EXIT_FAILURE);
			}
			if (listen(_listening_sockfds[i], 300) < 0) { // defines the maximum length to which the queue of pending connections for sockfd may grow.
				Utility::logger("Failed to listen on socket. errno: " +  Utility::to_string(errno), RED);
				std::exit(EXIT_FAILURE);
			}
			if (fcntl(_listening_sockfds[i], F_SETFL, O_NONBLOCK) == ERROR) {
				std::perror("fcntl error");
			}
			ListenInfo each_listen("0.0.0.0", _listen_ports[i]); //this struct will hold both ip and port info of running servers
			_running_servers[_listening_sockfds[i]] = each_listen;
			Utility::logger("Server listening on port: " + Utility::to_string(_listen_ports[i]), MAGENTA);
		}
	}

	bool Server::_is_in_listen_sockfd_list(int fd) {
		for(size_t i = 0; i < _listening_sockfds.size(); i++) {
			if(fd == _listening_sockfds[i]) {
				return true;
			}
		}
		return false;
	}

	void Server::_delete_events(int sock_kqueue, int identifier) {
		struct kevent kev;
		EV_SET(&kev, identifier, EVFILT_READ, EV_DELETE, 0, 0, NULL);
		kevent(sock_kqueue, &kev, 1, NULL, 0, NULL);
		EV_SET(&kev, identifier, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
		kevent(sock_kqueue, &kev, 1, NULL, 0, NULL);
	}

	void Server::_close_hanging_connections(int sock_kqueue) {
		if (!(_logtime_checker.should_check_hanging_connections())) {
			return;
		}
		std::map<int, Connection*>::iterator iter = _connections.begin();
		while (iter != _connections.end()) {
			if (iter->second->is_hanging_connection()) {
				if (iter->second->is_connection_open()) {
					iter->second->close();
					Utility::logger("Connection closed on timeout.", PURPLE); // for debug
				}
#ifdef _LINUX // manually removing an event from the kqueue as linux is not deleting it when a socket is closed
				_delete_events(sock_kqueue, temp_iter->first);
#endif
				iter = _destroy_connection(iter);
				// std::cout << "Connection destroyed by timer" << std::endl;
			} else {
				++iter;
			}
		}
		_logtime_checker.update_last_activity_logtime();
	}

		void Server::_remove_connection_closed_by_server(int sock_kqueue) {
		std::map<int, Connection*>::iterator iter = _connections.begin();
		while (iter != _connections.end()) {
			if (!(iter->second->is_connection_open())) {
#ifdef _LINUX // manually removing an event from the kqueue as linux is not deleting it when a socket is closed
				_delete_events(sock_kqueue, temp_iter->first);
#endif
				iter = _destroy_connection(iter);
			} else {
				++iter;
			}
		}
	}

	void Server::_remove_disconnected_client(int fd) {
		std::map<int, Connection*>::iterator iter = _connections.begin();
		while (iter != _connections.end()) {
			if (iter->first == fd) {
				_destroy_connection(iter);
				break;
			}
			iter++;
		}
	}

	std::map<int, Connection*>::iterator Server::_destroy_connection(std::map<int, Connection*>::iterator iterator) {
		std::cout << "Connection " << iterator->first << " destroyed\n";//TODO xiaojing the connection is closed before the message from child is processed and send back
		delete iterator->second;
		return _connections.erase(iterator);
	}

	void update_response_message(HTTPResponse::ResponseMessage &_http_response_message, std::string &response){
		std::string final_response;
		//set any remaining headers
		_http_response_message.set_header_element("Server", "HungerWeb/1.0");
		_http_response_message.set_header_element("Date", Utility::get_formatted_date());
		_http_response_message.set_header_element("Content-Length", Utility::to_string(response.length())); //TODO header is also included

		// build status line
		final_response += _http_response_message.get_HTTP_version() + " ";
		final_response += "200 ";
		final_response += "OK\r\n";

		// add all the headers to response. Format is {Header}: {Header value} \r\n
		for (std::map<std::string, std::string>::const_iterator it = _http_response_message.get_response_headers().begin(); it != _http_response_message.get_response_headers().end(); it++) {
			if (!it->first.empty())
				final_response += it->first + ": " + it->second;
			final_response += "\r\n";
		}

		// if body is not empty add it to  response. Format: \r\n {body}
		// response += "\r\n";
		final_response += response;
		std::cout << "final response" << final_response << std::endl;
		//final step
		_http_response_message.append_complete_response(final_response);
		// set the flag to true
	}

	void Server::_handle_events() {
		int new_events = 0;
		int sock_kqueue = kqueue(); //creates a new kernel event queue and returns a descriptor.
		if (sock_kqueue < 0) {
			Utility::logger("Error creating kqueue. errno: "  +  Utility::to_string(errno), RED);
			std::exit(EXIT_FAILURE);
		}
		struct kevent kev, event_fds; // First - kernel events we want to monitor, second - events triggered
		for(size_t i = 0; i < _listen_ports.size(); i++) {
			// Prepare a read event:
			EV_SET(&kev, _listening_sockfds[i], EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0); // is a macro which is provided for ease of initializing a kevent structure.
			// Register an event:
			if (kevent(sock_kqueue, &kev, 1, NULL, 0, NULL) < 0) {
				std::perror("kevent");
				std::exit(1);
			}
		}
		while (true) {
			struct timespec timeout;
			timeout.tv_sec = 30;
			timeout.tv_nsec = 0;
			// Receive events:
			new_events = kevent(sock_kqueue, NULL, 0, &event_fds, 1, &timeout); //look out for events and register to event list; one event per time
			if(new_events == -1) {
				std::cerr << "it is caused by new events register failure \n";
				std::perror("kevent");
				exit(1);
			}
			_close_hanging_connections(sock_kqueue);
			for (int i = 0; i < new_events; i++)
			{
				int current_event_fd = event_fds.ident;
				if (event_fds.flags & EV_ERROR) {
					std::cout << "Event error: %s", strerror(event_fds.data);
					std::exit(EXIT_FAILURE);
				}
				else if (event_fds.flags & EV_EOF) {
					Utility::logger("The client has disconnected.", RED);
					close(current_event_fd);
					_remove_disconnected_client(current_event_fd);
					Utility::logger("FD " + Utility::to_string(current_event_fd) + " is closed and removed from _connections." , RED);
				}
				else if(_is_in_listen_sockfd_list(current_event_fd)) { // if a new client is establishing a connection
					sockaddr_in connection_addr;
					int connection_addr_len = sizeof(connection_addr);
					int connection_socket_fd = accept(current_event_fd, (struct sockaddr *)&connection_addr, (socklen_t *)&connection_addr_len);
					if (connection_socket_fd == -1) {
						std::perror("accept socket error");
					}
					if (fcntl(connection_socket_fd, F_SETFL, O_NONBLOCK) == ERROR) {
						std::perror("fcntl error");
					}

					std::map<int, Connection *>::iterator it = _connections.find(connection_socket_fd);
					if (it != _connections.end()) {
						_destroy_connection(it);
					}

					Connection* connection_ptr = new Connection(connection_socket_fd, config_data, _running_servers[current_event_fd], connection_addr);
					_connections.insert(std::make_pair(connection_socket_fd, connection_ptr));
					Utility::logger("New connection on port  : " + Utility::to_string(_running_servers[current_event_fd].port), MAGENTA);

					// Register a read events for the client:
					EV_SET(&kev, connection_socket_fd, EVFILT_READ, EV_ADD, 0, 0, NULL); //put socket connection into the filter
					if (kevent(sock_kqueue, &kev, 1, NULL, 0, NULL) < 0) {
						std::perror("kevent error - read");
					}
				}
				else if (event_fds.filter == EVFILT_READ) { // if a read event is coming
					//steps for CGI handler to do after child returns the message body
					CGIHandler cgi_handler;
					int CGIReadFd = cgi_handler.get_read_fd();
					std::map<int, Connection*>::iterator connection_iter = _connections.find(current_event_fd);
					if(event_fds.ident == (uintptr_t)CGIReadFd){
						struct stat sb;
						std::string response;
						std::string final_response;
						fstat(CGIReadFd, &sb);
						response.resize(sb.st_size);
						read(CGIReadFd, (char*)(response.data()), sb.st_size);
						std::cout << "response message: " << response << std::endl;
						cgi_handler.set_response_message_body(response);
						close(CGIReadFd);
						// continue;
						std::cout << "check1\n";
						update_response_message(connection_iter->second->get_response_message(), response);
						std::cout << "check2\n";
						connection_iter->second->get_request_handler()->set_response_true();
						break;
					}
					else if (connection_iter != _connections.end()) { // handling request by the corresponding connection
						(connection_iter->second)->handle_http_request(sock_kqueue, cgi_handler);
						// Register write events for the client
						EV_SET(&kev, connection_iter->first, EVFILT_WRITE, EV_ADD, 0, 0, NULL); // is a macro which is provided for ease of initializing a kevent structure.
						if (kevent(sock_kqueue, &kev, 1, NULL, 0, NULL) < 0) {
							std::perror("kevent error - write");
						}
						new_events = kevent(sock_kqueue, NULL, 0, &event_fds, 1, NULL);
						if(new_events == -1) {
							std::perror("kevent");
							std::exit(1);
						}
						break;
					}
				}
				else if (event_fds.filter == EVFILT_WRITE) {
					std::map<int, Connection*>::iterator connection_iter = _connections.find(current_event_fd);
					if (connection_iter != _connections.end()) { // handling request by the corresponding connection
						connection_iter->second->send_response();
						if (!(connection_iter->second->is_connection_open())) {
							std::cout << "this is causing the segfault as the connection is desctoyed before the cgi output can be processed\n";//TODO discuss with @Olga this is causing the connection destroyed therefore segfault
							_destroy_connection(connection_iter);
						}
						break;
					}
				}
			}
		}
	}

	void Server::_setup_listening_ports() {
		const std::vector<Config::ServerBlock> servers = config_data->get_servers();
		for (size_t i = 0; i < servers.size(); i++)
		{
			std::set<std::string> listen_set = servers[i].get_listen();
			for (std::set<std::string>::iterator i = listen_set.begin(); i != listen_set.end(); i++) {
				int port;
				size_t pos = (*i).find("[::]:");
				if (pos != std::string::npos)
					port = std::atoi((*i).substr(pos + 5).c_str()); //if ipv6 port, remove the [::]:
				else
					port = std::atoi((*i).c_str()); //if ipv4
				if (std::find(_listen_ports.begin(), _listen_ports.end(), port) == _listen_ports.end()) //does not push duplicate ports
					_listen_ports.push_back(port);
			}
		}
	}

	void Server::run() {
		_setup_listening_ports();
		_setup_listening_sockets();
		_handle_events();
	}
}

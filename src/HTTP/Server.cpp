#include "Server.hpp"

#include <sys/socket.h> // for socket
#include <sys/errno.h>
#include <unistd.h> // for close
#include <iostream>
#include <string>
#include <algorithm>
#include <string.h> // strerror
#include <cstdlib> // for exit
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

	void Server::run() {
		_setup_listening_ports();
		_setup_listening_sockets();
		_handle_events();
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

	void Server::_setup_listening_sockets() {
		for(size_t i = 0; i < _listen_ports.size(); i++) {
			int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
			if (socket_fd < 0) {
				Utility::logger("Socket failed. errno: "  + Utility::to_string(errno), RED);
				std::exit(EXIT_FAILURE);
			}
			_listening_sockfds.push_back(socket_fd);
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
			if (fcntl(_listening_sockfds[i], F_SETFL, O_NONBLOCK) == Constants::ERROR) {
				std::perror("fcntl error");
			}
			ListenInfo each_listen("0.0.0.0", _listen_ports[i]); //this struct will hold both ip and port info of running servers
			_running_servers[_listening_sockfds[i]] = each_listen;
			Utility::logger("Server listening on port: " + Utility::to_string(_listen_ports[i]), MAGENTA);
		}
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
			if(new_events == Constants::ERROR) {
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
					_handle_disconnected_client(current_event_fd);
				}
				else if(_is_in_listen_sockfd_list(current_event_fd)) { // if a new client is establishing a connection
					_accept_new_connection(current_event_fd, sock_kqueue);
				}
				else if (event_fds.filter == EVFILT_READ) { // if a read event is coming
					_handle_read_event(current_event_fd, sock_kqueue);
				}
				else if (event_fds.filter == EVFILT_WRITE) {
					_handle_write_event(current_event_fd, sock_kqueue);
				}
			}
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
					Utility::logger("Connection " + Utility::to_string(iter->first) + " closed on timeout.", PURPLE); // for debug
				}
#ifdef _LINUX // manually removing an event from the kqueue as linux is not deleting it when a socket is closed
				_delete_events(sock_kqueue, temp_iter->first);
#endif
				iter = _destroy_connection(iter);
			} else {
				++iter;
			}
		}
		_logtime_checker.update_last_activity_logtime();
	}

	void Server::_handle_disconnected_client(int current_event_fd) {
		Utility::logger("The client " + Utility::to_string(current_event_fd) + " has disconnected.", BLUE);
		close(current_event_fd);
		_remove_disconnected_client(current_event_fd);
		Utility::logger("FD " + Utility::to_string(current_event_fd) + " is closed and removed from _connections." , BLUE);
	}

	void Server::_remove_disconnected_client(int fd) {
		std::map<int, Connection*>::iterator disconnected_client = _connections.find(fd);
		_destroy_connection(disconnected_client);
	}

	std::map<int, Connection*>::iterator Server::_destroy_connection(std::map<int, Connection*>::iterator iterator) {
		delete iterator->second;
		return _connections.erase(iterator);
	}

	void update_response_message(HTTPResponse::ResponseMessage& _http_response_message, std::string &response){
		//to remove the content type from the message body when calculating the message body length
		std::string final_response;
		std::size_t position = response.find("\r\n\r\n");
		std::string message_body =response;
		if(position != std::string::npos){
			message_body = response.substr(position + 4);
		}
		//set any remaining headers
		_http_response_message.set_header_element("Server", "HungerWeb/1.0");
		_http_response_message.set_header_element("Date", Utility::get_formatted_date());
		_http_response_message.set_header_element("Content-Length", Utility::to_string(message_body.length()));
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
		final_response += response;
		_http_response_message.append_complete_response(final_response);
	}

	void Server::_accept_new_connection(int current_event_fd, int sock_kqueue) {
		sockaddr_in connection_addr;
		int connection_addr_len = sizeof(connection_addr);
		int connection_socket_fd = accept(current_event_fd, (struct sockaddr *)&connection_addr, (socklen_t *)&connection_addr_len);
		if (connection_socket_fd == Constants::ERROR) {
			std::perror("accept socket error");
		}
		std::map<int, Connection *>::iterator it = _connections.find(connection_socket_fd);
		if (it != _connections.end()) {
			_destroy_connection(it);
		}

		Connection* connection_ptr = new Connection(connection_socket_fd, config_data, _running_servers[current_event_fd], connection_addr);
		_connections.insert(std::make_pair(connection_socket_fd, connection_ptr));
		Utility::logger("New connection " + Utility::to_string(connection_socket_fd) + " on port: " + Utility::to_string(_running_servers[current_event_fd].port), MAGENTA);

		// Register a read events for the client:
		struct kevent kev;
		EV_SET(&kev, connection_socket_fd, EVFILT_READ, EV_ADD, 0, 0, NULL); // put socket connection into the filter
		if (kevent(sock_kqueue, &kev, 1, NULL, 0, NULL) < 0) {
			std::perror("kevent error - read");
		}
	}

	void Server::_handle_read_event(int current_event_fd, int sock_kqueue) {
		std::map<int, Connection*>::iterator connection_iter = _connections.find(current_event_fd);
		if(connection_iter == _connections.end()) { // if the current fd is not the connection socket fd
			_handle_read_end_of_pipe();
		}
		else {
			(connection_iter->second)->handle_http_request(sock_kqueue);
			// Register write events for the client
			struct kevent kev;
			EV_SET(&kev, connection_iter->first, EVFILT_WRITE, EV_ADD, 0, 0, NULL); // is a macro which is provided for ease of initializing a kevent structure.
			if (kevent(sock_kqueue, &kev, 1, NULL, 0, NULL) < 0) {
				std::perror("kevent error - write");
			}
		}
	}

	void Server::_handle_write_event(int current_event_fd, int sock_kqueue) {
		std::map<int, Connection*>::iterator connection_iter = _connections.find(current_event_fd);
		if (connection_iter != _connections.end()) { // handling request by the corresponding connection
			connection_iter->second->send_response();
			if (!(connection_iter->second->is_connection_open())) {
				_destroy_connection(connection_iter);
			}
		}
		else {
			_handle_write_end_of_pipe(sock_kqueue);
		}
	}

	void Server::_handle_read_end_of_pipe() {
		struct stat sb;
		std::string response;
		std::string final_response;
		std::map<int, Connection*>::iterator it;
		for(it = _connections.begin(); it != _connections.end(); it++){
			int read_fd = it->second->get_cgi_read_fd();
			if(read_fd != -1) {
				if(fstat(read_fd, &sb) < 0){
					std::perror("fstat");
					it->second->handle_internal_server_error();
					it->second->set_response_true();//set the response ready to be send to client (500 error page)
				}
				response.resize(sb.st_size);
				int rt = read(read_fd, (char*)(response.data()), sb.st_size);
				if(rt < 0){
					std::perror("read");
					it->second->handle_internal_server_error();	
					it->second->set_response_true();//set the response ready to be send to client (500 error page)
					close(read_fd);
				}
				HTTPResponse::ResponseMessage& _http_response = it->second->get_response_message();
				update_response_message(_http_response, response);
				it->second->set_response_true();// set the flag to true
				break;
			}
		}
	}

	void Server::_handle_write_end_of_pipe(int sock_kqueue) {
		std::map<int, Connection *>::iterator it;
		for(it = _connections.begin(); it != _connections.end(); it++){
			int write_fd = it->second->get_cgi_write_fd();
			if(write_fd != -1){
				std::string request_message_body = it->second->get_request_message_body();
				int rt = write(write_fd, request_message_body.c_str(), request_message_body.size());
				if(rt < 0){
					std::perror("write error");
					it->second->handle_internal_server_error();
					it->second->set_response_true();//set the response ready to be send to client (500 error page)
				}
				else if(rt == 0 && request_message_body.size() != 0){//when the request message body size is zero write will return 0
					std::perror("write fd undefined");
					it->second->handle_internal_server_error();
					it->second->set_response_true();//set the response ready to be send to client (500 error page)
				}
				else{
					try{
						it->second->execute_cgi(sock_kqueue);	
					}
					catch(std::exception &e){
						it->second->handle_internal_server_error();
						it->second->set_response_true();//set the response ready to be send to client (500 error page)
					}
				}
				close(write_fd);
				it->second->set_cgi_write_fd(-1);
			}
		}
	}
}

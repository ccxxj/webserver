#pragma once

#include <netinet/in.h> // for sockaddr_in struct
#include <memory> // for auto_ptr

#include "RequestHandler.hpp"
#include "RequestHandlerDelegate.hpp"
#include "../Utility/SmartPointer.hpp"
#include "ServerStructs.hpp"

namespace HTTP {
	class Connection : public RequestHandlerDelegate
	{
	private:
		int _socket_fd;
		ListenInfo& _listen_info;
		bool _is_open;
		Utility::SmartPointer<RequestHandler> request_handler; // should be the last attr because of connection constructor
		sockaddr_in my_connection_addr;

	public:
		Connection(int connection_socket_fd, Config::ConfigData *config_data, ListenInfo& _listen_info, sockaddr_in connection_addr);
		~Connection();

<<<<<<< HEAD
		void handle_http_request(int kq);
		bool is_connection_open() const;
		virtual size_t receive(char *buffer, size_t buffer_size);
		virtual void send(const void *buffer, size_t buffer_size);
		virtual int get_fd();
=======
		sockaddr_in my_connection_addr;
		void handle_http_request();
		void send_response();
		bool is_connection_open() const;
		virtual size_t receive(char *buffer, size_t buffer_size);
		virtual void send(std::string& buffer, size_t buffer_size);
>>>>>>> origin
		virtual void close();
	};
}

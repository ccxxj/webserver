#pragma once

#include <netinet/in.h> // for sockaddr_in struct
#include <memory> // for auto_ptr

#include "RequestHandler.hpp"
#include "RequestHandlerDelegate.hpp"
#include "ServerStructs.hpp"
#include "../Utility/SmartPointer.hpp"
#include "../Utility/LogTimeCounter.hpp"

namespace HTTP {
	class Connection : public RequestHandlerDelegate
	{
	private:
		int _socket_fd;
		ListenInfo& _listen_info;
		bool _is_open;
		int _cgi_write_read_fd[2];//first number stores the write, second stores the read
		Utility::LogTimeCounter logtime_counter;
		Utility::SmartPointer<RequestHandler> request_handler;

	public:
		Connection(int connection_socket_fd, Config::ConfigData *config_data, ListenInfo& _listen_info, sockaddr_in connection_addr);
		~Connection();

		sockaddr_in my_connection_addr;
		void handle_http_request(int kq);
		void send_response();
		void set_cgi_write_fd(int i);
		void set_cgi_read_fd(int i);
		virtual int get_fd();
		bool is_connection_open() const;
		bool is_hanging_connection();
		void set_last_activity_time();
		int get_cgi_write_fd() const;
		int get_cgi_read_fd() const;
		HTTPResponse::ResponseMessage &get_response_message();
		Utility::SmartPointer<RequestHandler> get_request_handler();
		virtual size_t receive(char *buffer, size_t buffer_size);
		virtual void send(std::string& buffer, size_t buffer_size);
		virtual void close();
	};
}

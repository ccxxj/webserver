#include "Connection.hpp"

#include "../HTTPRequest/RequestParser.hpp"

namespace HTTP {
	Connection::Connection() {}
	Connection::Connection(int server_listening_sockfd) : _listening_socket_fd(server_listening_sockfd) {
		_client_addr_len = sizeof(_client_addr);
	}

	Connection::~Connection(){}

	void Connection::handle_http_request() {
		char buf[1024];
		memset(buf, '\0', 1024);
		size_t bytes_read = recv(_socket_fd, buf, sizeof(buf), 0);
		if (bytes_read > 0) 
		{
			        printf("read %zu bytes\n", bytes_read);
                    std::cout << "The message was:\n"<< std::endl;
                    std::cout.write(buf, bytes_read);
    
		}
		HTTPRequest::RequestParser parser;
		parser.parse_HTTP_request(buf, bytes_read);
		
		std::cout<< "\n\nrequest_message method is : " << parser.http_request_message.get_method() << std::endl;
		std::cout<< " request_message uri is : " << parser.http_request_message.get_request_uri() << std::endl;
		std::cout<< " request_message HTTP version is : " << parser.http_request_message.get_HTTP_version() << std::endl;
	}
}
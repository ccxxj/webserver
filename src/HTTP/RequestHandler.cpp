#include "RequestHandler.hpp"

#include "../HTTPRequest/RequestParser.hpp"

namespace HTTP {
	RequestHandler::RequestHandler(const Connection& active_connection):  _connection(active_connection){}

	RequestHandler::~RequestHandler()
	{
	}

	void RequestHandler::handle_http_request() {
		char buf[1024];
		memset(buf, '\0', 1024);
		size_t bytes_read = recv(_connection.get_socket_fd(), buf, sizeof(buf), 0);
		if (bytes_read > 0) 
		{
					printf("read %zu bytes\n", bytes_read);
					std::cout << "The message was:\n"<< std::endl;
					std::cout.write(buf, bytes_read);
	
		}
		HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
		parser.parse_HTTP_request(buf, bytes_read);
		
		std::cout<< "\n\nrequest_message method is : " << _http_request_message.get_method() << std::endl;
		std::cout<< " request_message uri is : " << _http_request_message.get_request_uri() << std::endl;
		std::cout<< " request_message HTTP version is : " << _http_request_message.get_HTTP_version() << std::endl;

		std::string response = "I do exist!\n";
		send(_connection.get_socket_fd(), response.c_str(), response.size(), 0);
	}
}

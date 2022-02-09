#include "RequestHandler.hpp"

#include <sstream> // for converting int to string
#include "../HTTPRequest/RequestParser.hpp"
#include "Exceptions/RequestException.hpp"

namespace HTTP {
	RequestHandler::RequestHandler(const Connection& active_connection):  _connection(active_connection){}

	RequestHandler::~RequestHandler(){}

	void RequestHandler::handle_http_request() {
		char buf[1024];
		memset(buf, '\0', 1024);
		size_t bytes_read = recv(_connection.get_socket_fd(), buf, sizeof(buf), 0);
		// TODO: add check on the bytes read == -1 or == 0(if the client stopped the connection)
		if (bytes_read > 0)
		{
			printf("read %zu bytes\n", bytes_read);
			std::cout << "The message was:\n"<< std::endl;
			std::cout.write(buf, bytes_read);
		}
		try {

			HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
			parser.parse_HTTP_request(buf, bytes_read);
			
			std::cout<< "\n\nrequest_message method is : " << _http_request_message.get_method() << std::endl;
			std::cout<< " request_message uri is : " << _http_request_message.get_request_uri() << std::endl;
			std::cout<< " request_message HTTP version is : " << _http_request_message.get_HTTP_version() << std::endl;

		}
		catch(const Exception::RequestException& e)
		{
			_handle_exception(e.get_error_status_code());
		}
		std::string response = "I do exist!\n";
		send(_connection.get_socket_fd(), response.c_str(), response.size(), 0);
	}

	void RequestHandler::_handle_exception(const int code) {
		_http_response_message.set_status_code(_convert_status_code_to_string(code));
		_http_response_message.set_reason_phrase(HTTPResponse::get_reason_phrase(HTTPResponse::StatusCode.code));
	}

	const std::string& RequestHandler::_convert_status_code_to_string(const int code) {
		std::string stringified_code;
		std::stringstream sstream;
		sstream << code;
		sstream >> stringified_code;
		return stringified_code;
	}
}


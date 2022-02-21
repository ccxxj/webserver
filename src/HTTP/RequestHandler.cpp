#include "RequestHandler.hpp"

#include <sstream> // for converting int to string
#include <stdio.h> // for perror

#include "Exceptions/RequestException.hpp"
#include "../globals.hpp"

namespace HTTP {
	RequestHandler::RequestHandler(const Connection& active_connection)
	: _http_request_message()
	, _http_response_message()
	, _connection(active_connection)
	, _parser(&_http_request_message, &_http_response_message)
	{
	}

	RequestHandler::~RequestHandler(){}

	void RequestHandler::handle_http_request() {
		char buf[4096];
		ssize_t bytes_read = _connection.recv(buf, sizeof(buf));
		if (bytes_read == 0) {
			_connection.close();
		} else if (bytes_read == ERROR) {
			perror("recv error");
			_connection.close();
		} else {
				std::cout << "\nRead " << bytes_read << " bytes\n";
				std::cout.write(buf, bytes_read);
				try {
					_parser.parse_HTTP_request(buf, bytes_read);
				}
				catch(const Exception::RequestException& e)
				{
					_handle_request_exception(e.get_error_status_code());
				}
				if (!_parser.is_parsing_finished()) {
					return;
				}
			std::string status_code = _http_response_message.get_status_code();
			std::string reason_phrase = _http_response_message.get_reason_phrase();
			std::string status_line = _http_response_message.get_HTTP_version() + " " + status_code + " " + reason_phrase + "\r\n\r\n";
			std::cout << "\n\nstatus line: " << status_line << std::endl;
			_connection.send(&status_line[0], status_line.size()); //TODO: replace by full response
			_connection.close();
		}
	}

	void RequestHandler::_handle_request_exception(HTTPResponse::StatusCode code) {
		_http_response_message.set_status_code(_convert_status_code_to_string(static_cast<int>(code)));
		_http_response_message.set_reason_phrase(HTTPResponse::get_reason_phrase(code));
		std::cout << "\n\nstatus code: " << _http_response_message.get_status_code() << std::endl;
		std::cout << "\n\nreason phrase: " << _http_response_message.get_reason_phrase() << std::endl;
	}

	const std::string RequestHandler::_convert_status_code_to_string(const int code) {
		std::string stringified_code;
		std::stringstream sstream;
		sstream << code;
		sstream >> stringified_code;
		return stringified_code;
	}
}

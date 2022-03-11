#include "RequestHandler.hpp"

#include <sstream> // for converting int to string
#include <stdio.h> // for perror

#include "Exceptions/RequestException.hpp"
#include "../globals.hpp"

namespace HTTP {
	RequestHandler::RequestHandler(RequestHandlerDelegate& delegate)
	: _http_request_message()
	, _http_response_message()
	, _delegate(delegate)
	, _parser(&_http_request_message, &_http_response_message)
	{
	}

	RequestHandler::~RequestHandler(){}

	void RequestHandler::handle_http_request(const int connection_socket_fd) { //TODO do i need the socket_fd?
		char buf[4096];
		ssize_t bytes_read = _delegate.receive(buf, sizeof(buf));
		if (bytes_read == 0) {
			_delegate.close();
		} else if (bytes_read == ERROR) {
			perror("recv error");
			_delegate.close();
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
			//TODO process the request and connecto ConfigData to here somewhere
			_process_request_message();
			std::string status_code = _http_response_message.get_status_code();
			std::string reason_phrase = _http_response_message.get_reason_phrase();
			std::string status_line = _http_response_message.get_HTTP_version() + " " + status_code + " " + reason_phrase + "\r\n\r\n";
			std::cout << "\n\nRESPONSE:\nStatus line: " << status_line << std::endl;
			_delegate.send(&status_line[0], status_line.size()); //TODO: replace by full response
			_delegate.close();
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

	void RequestHandler::_process_http_request() {
		// ServerBlock virtual_server = find_virtual_server(); (if no match return the default ServerBlock)
		// LocationBlock location_conf = get_location_conf(virtual server);
		// if( !verify_method(location_conf.get_limit_except()))
			//method not allowed error

		// create_response(); // finalizing_request!
			// get_http_version();
			// get_response_headers();
		//std::string host = _http_request_message.get_header_value("Host");
		//if (host.empty()) //TODO no host header error?
	}
}

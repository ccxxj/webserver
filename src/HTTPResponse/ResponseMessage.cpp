#include "ResponseMessage.hpp"

namespace HTTPResponse {
    ResponseMessage::ResponseMessage()
        : _HTTP_version("HTTP/1.1")
        , _status_code("")
        , _reason_phrase("")
    {}

    ResponseMessage::ResponseMessage(const ResponseMessage& other)
        : _HTTP_version("HTTP/1.1")
        , _status_code(other._status_code)
        , _reason_phrase(other._reason_phrase)
        , _message_body(other._message_body)
        , _complete_response(other._complete_response)
        , _response_headers(other._response_headers)
    {}

    ResponseMessage::~ResponseMessage() {}

    void ResponseMessage::set_status_code(const std::string& code) {
        _status_code = code;
    }

    void ResponseMessage::set_reason_phrase(const std::string& reason) {
        _reason_phrase = reason;
    }

	void ResponseMessage::set_message_body(const std::string& body) {
        _message_body = body;
    }

    void ResponseMessage::set_complete_response(const std::string& response) {
        _complete_response = response;
    }

    void ResponseMessage::append_complete_response(const std::string& response_part) {
        _complete_response +=  response_part;
    }

	void ResponseMessage::set_header_element(std::string header, std::string value) {
		std::pair<std::string, std::string> header_field(header, value);
		_response_headers.insert(header_field);
	}

    const std::string& ResponseMessage::get_HTTP_version() const {
        return _HTTP_version;
    }

    const std::string& ResponseMessage::get_status_code() const {
        return _status_code;
    }

    const std::string& ResponseMessage::get_reason_phrase() const {
        return _reason_phrase;
    }

    const std::string& ResponseMessage::get_message_body() const {
         return _message_body;
    }

    std::string& ResponseMessage::get_complete_response() {
        return _complete_response;
    }

	const std::map<std::string, std::string>& ResponseMessage::get_response_headers() const {
		return _response_headers;
	}
} // namespace HTTPResponse


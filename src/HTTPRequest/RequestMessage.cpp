#include "RequestMessage.hpp"

namespace HTTPRequest {
    RequestMessage::RequestMessage() : _method(""), _request_uri(""), _HTTP_version(""), _message_body(""){}

    RequestMessage::~RequestMessage() {}

    const std::string& RequestMessage::get_method() const {
        return _method;
    }

    void RequestMessage::set_method(std::string& method) {
        _method = method;
    }
    const std::string& RequestMessage::get_request_uri() const {
        return _request_uri;
    }

    void RequestMessage::set_request_uri(std::string& request_uri) {
        _request_uri = request_uri;
    }

    const std::string& RequestMessage::get_HTTP_version() const {
        return _HTTP_version;
    }
    void RequestMessage::set_HTTP_version(std::string& version) {
        _HTTP_version = version;
    }

    const std::string& RequestMessage::get_header_value(const std::string& header_name) const {
        return (*_request_headers.find(header_name)).second;
    }

//debug
    std::map<std::string, std::string> RequestMessage::get_headers() const {
        return _request_headers;
    }

    void RequestMessage::set_header_field(std::pair<std::string, std::string>& header_field) {
        std::map<std::string, std::string>::iterator header_name_position = _request_headers.find(header_field.first);
        if ( header_name_position == _request_headers.end()) {
            _request_headers.insert(header_field);
        }
        else {
            header_name_position->second.append(", ");
            header_name_position->second.append(header_field.second);
        }
    }
}

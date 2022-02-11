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

    void RequestMessage::set_header_field(std::pair<std::string, std::string>& header_field){
        _request_headers.insert(header_field); //TODO: or use operator[] ?
    }
}

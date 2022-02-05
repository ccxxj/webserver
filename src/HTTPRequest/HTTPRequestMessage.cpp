#include "HTTPRequestMessage.hpp"

HTTPRequestMessage::HTTPRequestMessage() : _method(""), _request_uri(""), _HTTP_version(""), _message_body(""){
    
}

 HTTPRequestMessage::~HTTPRequestMessage() {}

    const std::string& HTTPRequestMessage::get_method() const {
        return _method;
    }

    void HTTPRequestMessage::set_method(std::string& method) {
        _method = method;
    }
    const std::string& HTTPRequestMessage::get_request_uri() const {
        return _request_uri;
    }

    void HTTPRequestMessage::set_request_uri(std::string& request_uri) {
        _request_uri = request_uri;
    }

    const std::string& HTTPRequestMessage::get_HTTP_version() const {
        return _HTTP_version;
    }
    void HTTPRequestMessage::set_HTTP_version(std::string& version) {
        _HTTP_version = version;
    }

    const std::string& HTTPRequestMessage::get_header_value(const std::string& header_name) const {
        return (*_request_headers.find(header_name)).second;
    }

    void HTTPRequestMessage::set_header_field(std::pair<std::string, std::string>& header_field){
        _request_headers.insert(header_field); //TODO: or use operator[] ?
    }
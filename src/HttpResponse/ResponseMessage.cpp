#include "ResponseMessage.hpp"

namespace HTTPResponse {
    ResponseMessage::ResponseMessage() : _HTTP_version("HTTP/1.1"), _status_code(""), _reason_phrase(""){}

    ResponseMessage::~ResponseMessage() {}

    void ResponseMessage::set_status_code(const std::string& code) {
        _status_code = code;
    }
    
    void ResponseMessage::set_status_code(const std::string& reason) {
        _reason_phrase = reason;
    }
    

}
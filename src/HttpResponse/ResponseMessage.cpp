#include "ResponseMessage.hpp"

namespace HTTPResponse {
    ResponseMessage::ResponseMessage() : _HTTP_version("HTTP/1.1"), _status_code(""), _reason_phrase(""){}

    ResponseMessage::~ResponseMessage() {}
}
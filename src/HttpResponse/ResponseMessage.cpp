#include "ResponseMessage.hpp"

namespace HTTPResponse {
    ResponseMessage::ResponseMessage() : _HTTP_version(""), _status_code(""), _reason_phrase(""){}

    ResponseMessage::~ResponseMessage() {}
}
#include "RequestException.hpp"

namespace Exception {
    RequestException::RequestException(HTTPResponse::StatusCode error_code) : _error_status_code(error_code){}

    RequestException::~RequestException(){}

    HTTPResponse::StatusCode RequestException::get_error_status_code() const {
        return _error_status_code;
    }
}
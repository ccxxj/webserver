#include "RequestException.hpp"

namespace Exception {
    RequestException::RequestException(int error_code) : _error_status_code(error_code){}

    RequestException::~RequestException(){}

    const int RequestException::get_error_status_code() const {
        return _error_status_code;
    }
}
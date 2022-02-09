#include "RequestException.hpp"

namespace Exception {
    RequestException::RequestException(int error_code) : _error_status_code(error_code){}

    RequestException::~RequestException(){}
}
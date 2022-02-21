#ifndef REQUESTEXCEPTION_HPP
#define REQUESTEXCEPTION_HPP

#include <string>
#include "../../HTTPResponse/StatusCodes.hpp"

namespace Exception {

    class RequestException {

    private:
        HTTPResponse::StatusCode _error_status_code;

    public:
        RequestException(HTTPResponse::StatusCode error_code);
        ~RequestException();

        HTTPResponse::StatusCode get_error_status_code() const;
    };
}

#endif

#ifndef REQUESTEXCEPTION_HPP
#define REQUESTEXCEPTION_HPP

#include <string>

namespace Exception {

    class RequestException
    {
    public:
        RequestException(int error_code);
        ~RequestException();
    private:
        int _error_status_code;
    };
}

#endif
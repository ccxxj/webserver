#ifndef REQUESTEXCEPTION_HPP
#define REQUESTEXCEPTION_HPP

#include <string>

namespace Exception {

    class RequestException
    {
    public:
        RequestException(int error_code);
        ~RequestException();

        const int get_error_status_code() const;
    private:
        int _error_status_code;
    };
}

#endif

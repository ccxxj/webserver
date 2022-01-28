#ifndef ALLOWED_REQUEST_METHODS_HPP
#define ALLOWED_REQUEST_METHODS_HPP
#include <string>

class Allowed_Request_Methods {
    Allowed_Request_Methods();
    ~Allowed_Request_Methods();

private:      
    static const std::string _GET;
    static const std::string _POST;
    static const std::string _HEAD;
    static const std::string _PUT;
    static const std::string _DELETE;
    static const std::string _CONNECT;
    static const std::string _OPTIONS;
    static const std::string _TRACE;
    static const std::string _PATCH;
};

 #endif

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP
#include <string>
#include <map>

class HTTPRequest {

public:
    HTTPRequest();
    HTTPRequest(const HTTPRequest& other);
    ~HTTPRequest();
    const HTTPRequest& operator=(const HTTPRequest& other);

    const std::string& get_method() const;
    void set_method(std::string& method);
    const std::string& get_request_uri() const;
    void set_request_uri(std::string& method);
    const std::string& get_HTTP_version() const;
    void set_HTTP_version(std::string& method);

private:
    std::string _method;
    std::string _request_uri;
    std::string _HTTP_version;
    std::map<std::string, std::string> _request_headers;
    std::string _message_body;
};


#endif

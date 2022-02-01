#ifndef HTTPREQUESTMESSAGE_HPP
#define HTTPREQUESTMESSAGE_HPP
#include <string>
#include <map>

class HTTPRequestMessage {

public:
    HTTPRequestMessage();
    HTTPRequestMessage(const HTTPRequestMessage& other);
    ~HTTPRequestMessage();
    const HTTPRequestMessage& operator=(const HTTPRequestMessage& other);

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

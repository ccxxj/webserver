#ifndef RequestMessage_HPP
#define RequestMessage_HPP
#include <string>
#include <map>

#include "URI/URIData.hpp"

namespace HTTPRequest {
    class RequestMessage {

    private:
        std::string _method;
        std::string _request_uri;
        URIData uri_data;
        std::string _HTTP_version;
        std::map<std::string, std::string> _request_headers;
        std::string _message_body;

    public:
        RequestMessage();
        RequestMessage(const RequestMessage& other);
        ~RequestMessage();
        const RequestMessage& operator=(const RequestMessage& other);

        const std::string& get_method() const;
        void set_method(std::string& method);
        void set_uri(URIData &uri);
        const URIData &get_uri(void) const;
        const std::string& get_request_uri() const;
        void set_request_uri(std::string& request_uri);
        const std::string& get_HTTP_version() const;
        void set_HTTP_version(std::string& version);
        const std::string& get_header_value(const std::string& header_name) const;
        void set_header_field(std::pair<std::string, std::string>& header_field);
        const std::string& get_message_body() const;
        void set_message_body(std::string& body);

        //debug
        std::map<std::string, std::string> get_headers() const; //TODO;; temp?
    };
}
#endif

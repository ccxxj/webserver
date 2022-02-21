#ifndef RESPONSEMESSAGE_HPP
#define RESPONSEMESSAGE_HPP
#include <string>
#include <map>

namespace HTTPResponse {

    class ResponseMessage {

    private:
        std::string _HTTP_version;
        std::string _status_code;
        std::string _reason_phrase;
        std::map<std::string, std::string> _request_headers;
        std::string _message_body;

    public:
        ResponseMessage();
        ResponseMessage(const ResponseMessage& other);
        ~ResponseMessage();

        void set_status_code(const std::string& code);
        void set_reason_phrase(const std::string& reason);
        const std::string get_HTTP_version() const;
        const std::string get_status_code() const;
        const std::string get_reason_phrase() const;
    };
}

#endif

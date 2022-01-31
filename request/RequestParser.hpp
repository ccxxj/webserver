#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP
#include <string>
#include <set>
#include <vector>
#include <iostream> // TODO: remove

class RequestParser {

public:
    RequestParser();
    RequestParser(const RequestParser& other);
    ~RequestParser();
    const RequestParser& operator=(const RequestParser& other);


    static std::string& parse_method(const std::string& message_buffer);
    static std::string& parse_request_uri(const std::string& message_buffer);
    static std::string& parse_HTTP_version(const std::string& message_buffer);
    static std::string& parse_headers(const std::string& message_buffer);
    static std::string& parse_message_body(const std::string& message_buffer);
private:
    const std::string _request_line;


private:
    enum State {
        METHOD = 0,
        REQUEST_URI,
        HTTP_VERSION,
        HEADERS,
        MESSAGE_BODY,
        NUM_STATES
    };

    enum TriggerEvent {
        SPACE_FOUND = 0,
        NEWLINE_FOUND,
        EMPTY_LINE_FOUND,
        NUM_EVENTS
    };

    struct StateDispatcher {
        enum State state;
        std::string& (*ptr)(const std::string& value);
    };

    static StateDispatcher state_dispatch_table[];

    const std::string& _parse_request_line(char* buffer);
};


#endif

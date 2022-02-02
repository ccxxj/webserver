#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <string>
#include <set>
#include <vector>
#include <iostream> // TODO: remove

#include "RequestReader.hpp"
#include "HTTPRequestMessage.hpp"

class RequestParser {

public:
    RequestParser();
    RequestParser(const RequestParser& other);
    ~RequestParser();
    const RequestParser& operator=(const RequestParser& other);

    void parse_HTTP_request(char* buffer);

// TODO: check the smaller fucntionality:
private:
    RequestReader request_reader;
    HTTPRequestMessage http_request_message;

    void _parse_request_line(const std::string& accumulating_string);
    void _parse_headers(const std::string& accumulaing_string);

    std::string& _parse_method(const std::string& message_buffer);
    std::string& parse_request_uri(const std::string& message_buffer);
    std::string& parse_HTTP_version(const std::string& message_buffer);
    std::string& parse_message_body(const std::string& message_buffer);

   std::string _get_substring(size_t* start, const std::string& string);

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

};


#endif

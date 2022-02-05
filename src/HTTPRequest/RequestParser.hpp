#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <string>
#include <set>
#include <vector>
#include <iostream> // TODO: remove

#include "RequestReader.hpp"
#include "HTTPRequestMessage.hpp"

namespace HTTPRequest {

    class RequestParser {

    public:
        RequestParser();
        RequestParser(const RequestParser& other);
        ~RequestParser();
        const RequestParser& operator=(const RequestParser& other);

        void parse_HTTP_request(char* buffer, size_t bytes_read);
        HTTPRequestMessage http_request_message;


    private:
        RequestReader request_reader;

        void _parse_request_line(const std::string& accumulating_string);
        void _parse_header(const std::string& accumulating_string);

        std::vector<std::string> split(const std::string& accumulating_string);
        
        std::string &_parse_method(const std::string &message_buffer);
        std::string& parse_request_uri(const std::string& message_buffer);
        std::string& parse_HTTP_version(const std::string& message_buffer);
        std::string& parse_message_body(const std::string& message_buffer);

    std::string _get_substring(size_t* start, const std::string& string);

    };
}


#endif

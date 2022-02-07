#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <string>
#include <set>
#include <vector>
#include <iostream> // TODO: remove

#include "RequestReader.hpp"
#include "RequestMessage.hpp"
#include "../HTTPResponse/ResponseMessage.hpp"
#include "../HTTPResponse/StatusCodes.hpp"

namespace HTTPRequest {

    class RequestParser {

    public:
        RequestParser(HTTPRequest::RequestMessage* http_request, HTTPResponse::ResponseMessage* http_response);
        RequestParser(const RequestParser& other);
        ~RequestParser();
        const RequestParser& operator=(const RequestParser& other);

        void parse_HTTP_request(char* buffer, size_t bytes_read);
        HTTPRequest::RequestMessage* _http_request_message;
        HTTPResponse::ResponseMessage* _http_response_message;
    private:
        RequestParser();
        RequestReader request_reader;

        void _parse_request_line(const std::string& accumulating_string);
        void _parse_header(const std::string& accumulating_string);

        std::vector<std::string> _split_line(const std::string& line, const char delimiter);

    };
}


#endif

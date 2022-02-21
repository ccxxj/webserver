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

    private:
        RequestReader _request_reader;
        std::string _accumulator;

        void _parse_request_line();
        void _parse_header();
        bool _is_method_supported(const std::string& method);
        size_t _longest_method_size();

        RequestParser();
        std::vector<std::string> _split_line(const std::string& line, const char delimiter);
        std::string _trim(const std::string& s);

        bool _comp(const std::string& lhs, const std::string& rhs);
    public:
        HTTPRequest::RequestMessage* _http_request_message;
        HTTPResponse::ResponseMessage* _http_response_message;

        RequestParser(HTTPRequest::RequestMessage* http_request, HTTPResponse::ResponseMessage* http_response);
        RequestParser(const RequestParser& other);
        ~RequestParser();
        const RequestParser& operator=(const RequestParser& other);

        void parse_HTTP_request(char* buffer, size_t bytes_read);
    };
}

#endif

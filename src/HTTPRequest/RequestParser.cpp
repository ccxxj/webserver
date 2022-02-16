#include "RequestParser.hpp"
#include <algorithm> // for std::distance

#include "../HTTP/Exceptions/RequestException.hpp"
#include "HTTPRequestMethods.hpp"

namespace HTTPRequest {

    RequestParser::RequestParser(HTTPRequest::RequestMessage* http_request, HTTPResponse::ResponseMessage* http_response)
        : _http_request_message(http_request)
        , _http_response_message(http_response) {}

    RequestParser::~RequestParser(){}

    // TODO: check for any whitespaces which are not allowed
    void RequestParser::parse_HTTP_request(char* buffer, size_t bytes_read) {
        char *message_end = buffer + bytes_read;
        _accumulator = _request_reader.read_line(&buffer, message_end);
        _parse_request_line();
        //TODO: validate request line
        while(buffer != message_end) {
            if (_request_reader._is_end_of_header_fields(buffer, message_end)){
                buffer += 2; // skipping only one \r\n as the previous one has allrady been skipped in the previous iteration
                break;
            }
            _accumulator = _request_reader.read_line(&buffer, message_end);
            _parse_header();
        }
        //TODO: validate headers
    }

    std::vector<std::string> RequestParser::_split_line(const std::string& line, const char delimiter){
        size_t start  = 0;
        std::vector<std::string> lines;

        while (true)
        {
            size_t match = line.find(delimiter, start);
            if (match == std::string::npos) {
                break;
            }
            size_t len = match - start;
            lines.push_back(line.substr(start, len));
            start = match + 1;
        }
        lines.push_back(line.substr(start, line.size()));
        return lines;
    }

    bool RequestParser::_comp(const std::string& lhs, const std::string& rhs) {
        return (lhs.size() < rhs.size());
    }

    size_t RequestParser::_longest_method_size() {
        size_t longest_size = 0;
        for (std::set<std::string>::iterator it = HTTPRequestMethods.begin(); it != HTTPRequestMethods.end(); it++) {
            size_t string_size = (*it).size();
            if ( string_size > longest_size) {
                longest_size = string_size;
            }
        }
        return longest_size;
    }

    bool RequestParser::_is_method_supported(const std::string& method) {
        if (method.size() > _longest_method_size()){
            throw Exception::RequestException(HTTPResponse::NotImplemented);
        }
        if (HTTPRequestMethods.find(method) == HTTPRequestMethods.end()) {
            throw Exception::RequestException(HTTPResponse::BadRequest);
        }
        return true;
    }

    void RequestParser::_parse_request_line() {
        if (_accumulator.empty())
            throw Exception::RequestException(HTTPResponse::BadRequest);
        std::vector<std::string> segments = _split_line(_accumulator, ' ');
        if (segments.size() < 3) {
            throw Exception::RequestException(HTTPResponse::BadRequest);
        }
        if (_is_method_supported(segments[0])) {
            _http_request_message->set_method(segments[0]);
        }
        if (segments[1].size() > 2000) {
            throw Exception::RequestException(HTTPResponse::URITooLong);
        }
        _http_request_message->set_request_uri(segments[1]);
        _http_request_message->set_HTTP_version(segments[2]);
    }

    void RequestParser::_parse_header() {
            if (_accumulator == "")
            return; //TODO: fill error response
        std::vector<std::string> segments = _split_line(_accumulator, ':');
        std::pair<std::string, std::string> header_field(segments[0], _trim(segments[1])); //TODO: No whitespace is allowed between the header field-name and colon.
        _http_request_message->set_header_field(header_field); //TODO: what if the header name exists?
    }
 
    std::string RequestParser::_trim(const std::string& s)
    {
        std::string::const_iterator start = s.begin();
        while (start != s.end() && std::isspace(*start)) {
            start++;
        }
        std::string::const_iterator end = s.end();
        do {
            end--;
        } while (std::distance(start, end) > 0 && std::isspace(*end));
    
        return std::string(start, end + 1);
    }
}

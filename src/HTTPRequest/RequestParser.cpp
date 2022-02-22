#include "RequestParser.hpp"
#include <algorithm> // for std::distance

#include "../HTTP/Exceptions/RequestException.hpp"
#include "HTTPRequestMethods.hpp"


namespace HTTPRequest {

    RequestParser::Dispatch RequestParser::_dispatch_table[] = {
        {REQUEST_LINE, &RequestParser::_parse_request_line},
        {HEADER, &RequestParser::_parse_header},
        {MESSAGE_BODY, &RequestParser::_parse_message_body},
        {FINISHED, NULL}
    };

    RequestParser::RequestParser(HTTPRequest::RequestMessage* http_request, HTTPResponse::ResponseMessage* http_response)
        : _current_parsing_state(REQUEST_LINE)
        , _http_request_message(http_request)
        , _http_response_message(http_response){}

    RequestParser::~RequestParser(){}

    void RequestParser::_handle_request_message_part(std::string& line) {
        Dispatch *message = _dispatch_table;
        for (size_t i = 0; message[i].parsing_state != FINISHED; ++i) {
            if (message[i].parsing_state == _current_parsing_state) {
                (this->*(message[i].ptr))(line);
                return;
            }
        }
    }

    bool RequestParser::is_parsing_finished() {
        return _current_parsing_state == FINISHED;
    }

    // TODO: check for any whitespaces which are not allowed
    void RequestParser::parse_HTTP_request(char* buffer, size_t bytes_read) {
        char *buffer_end = buffer + bytes_read;
        while (buffer != buffer_end || _current_parsing_state != FINISHED) {
            bool can_be_parsed = false;
            std::string line = _request_reader.read_line(&buffer, buffer_end, &can_be_parsed);
            if (can_be_parsed == false)
            {
                return;
            }
            else
            {
                _handle_request_message_part(line);
            }
        }
        //TODO: validate request line
        //TODO: validate headers
    }

    void RequestParser::_throw_request_exception(HTTPResponse::StatusCode error_status) {
        _current_parsing_state = FINISHED;
        throw Exception::RequestException(error_status);
    }

    bool RequestParser::contains_whitespace(std::string& str) {
        size_t whitespace_found = str.find(' ', 0);
        size_t tab_found = str.find('\t', 0);
        return whitespace_found != std::string::npos || tab_found != std::string::npos;
    }

    void RequestParser::_parse_request_line(std::string& line) {
        if (line.empty()) {

            _throw_request_exception(HTTPResponse::BadRequest);
        }
        std::vector<std::string> segments = _split_line(line, ' ');
        if (segments.size() != 3) {
            _throw_request_exception(HTTPResponse::BadRequest);
        }
        if (_is_method_supported(segments[0])) {
            _http_request_message->set_method(segments[0]);
        }
        if (segments[1].size() > 2000) {
            _throw_request_exception(HTTPResponse::URITooLong);
        }
        _http_request_message->set_request_uri(segments[1]);
        _http_request_message->set_HTTP_version(segments[2]);
        _current_parsing_state = HEADER;
    }

    bool RequestParser::_is_method_supported(const std::string& method) {
        if (method.size() > _longest_method_size()){
            _throw_request_exception(HTTPResponse::NotImplemented);
        }
        if (HTTPRequestMethods.find(method) == HTTPRequestMethods.end()) {
            _throw_request_exception(HTTPResponse::BadRequest);
        }
        return true;
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

    void RequestParser::_parse_header(std::string& line) {
        if (line == "\r\n" || line == "") {
            _current_parsing_state = FINISHED; // TODO: change to message body after headers and request line are validated
            // _current_parsing_state = MESSAGE_BODY;
            return;
        }
        std::vector<std::string> segments = _split_line(line, ':');
        if (contains_whitespace(segments[0])) {
            _throw_request_exception(HTTPResponse::BadRequest);
        }
        std::pair<std::string, std::string> header_field(segments[0], _trim(segments[1]));
        _http_request_message->set_header_field(header_field);
    }

    void RequestParser::_parse_message_body(std::string& line) {
        _current_parsing_state = FINISHED;
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

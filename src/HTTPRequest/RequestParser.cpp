#include "RequestParser.hpp"
#include <algorithm> // for std::distance
#include <cstdlib> // for atoi

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

    int RequestParser::_set_content_length() {
        std::string content_length_value = _http_request_message->get_headers().find("Content-Length")->second;
        if (content_length_value.find_first_of(',', 0) != std::string::npos) {
            std::vector<std::string> values = _split_line(content_length_value, ',');
            std::string first_value = _trim(values[0]);
            for (size_t i = 1; i < values.size(); ++i) {
                if (first_value != _trim(values[i])) {
                    _throw_request_exception(HTTPResponse::BadRequest);
                }
            }
            content_length_value = first_value;
        }
        for (std::string::iterator it = content_length_value.begin(); it != content_length_value.end(); ++it) {
            if (!isdigit(*it)) {
                _throw_request_exception(HTTPResponse::BadRequest); // should throw for negative values as well
            }
        }
        return std::atoi(content_length_value.c_str());
    }

    void RequestParser::parse_HTTP_request(char* buffer, size_t bytes_read) {
        char* buffer_end = buffer + bytes_read;
        size_t content_length = 0;
        while (buffer != buffer_end || _current_parsing_state != FINISHED)
        {
            bool can_be_parsed = false;
            std::string line = _request_reader.read_line(&buffer, buffer_end, &can_be_parsed);
            if (_current_parsing_state == MESSAGE_BODY && line.size() == content_length) {
                can_be_parsed = true;
            }
            if (can_be_parsed == false) {
                return;
            }
            else {
                _handle_request_message_part(line);
                if (_current_parsing_state == MESSAGE_BODY) {
                    _define_message_body_length();
                    if (_message_body_length == CONTENT_LENGTH) {
                        content_length = _set_content_length();
                        buffer_end = buffer + content_length;
                    }
                    //TODO: validate request line
                    //TODO: validate headers
                    //TODO: what if content-length is less than bytes read? Do we close the connection or need to support keep-alive?
                }
            }
        }
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
        _http_request_message->set_request_uri(segments[1]);

        URIParser uri_parser(segments[1]);
        URIData uri_data;
        uri_parser.parse(uri_data);
        _http_request_message->set_uri(uri_data);
        
        _http_request_message->set_HTTP_version(segments[2]);
        _current_parsing_state = HEADER;
    }
    
    bool RequestParser::_is_method_supported(const std::string& method) {
        if (method.size() > _longest_method_size()) {
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
            _current_parsing_state = MESSAGE_BODY;
            return;
        }
        std::vector<std::string> segments = _split_line(line, ':');
        if (contains_whitespace(segments[0])) {
            _throw_request_exception(HTTPResponse::BadRequest);
        }
        std::pair<std::string, std::string> header_field(segments[0], _trim(segments[1]));
        _http_request_message->set_header_field(header_field);
    }

    ssize_t RequestParser::_find_chuncked_encoding_position(std::vector<std::string>& encodings, size_t encodings_num) {
        for (size_t i = 0; i < encodings_num; ++i) {
            if (_trim(encodings[i]) == "chunked") {
                return i;
            }
        }
        return -1;
    }

    void RequestParser::_delete_obolete_content_length_header() {
        _http_request_message->get_headers().erase("Content-Length");
    }

    void RequestParser::_parse_transfer_encoding(std::string coding_names_list) {
        std::vector<std::string> encodings = _split_line(coding_names_list, ','); //splitting the header value
        ssize_t encodings_num = encodings.size();
        ssize_t chuncked_position = _find_chuncked_encoding_position(encodings, encodings_num);
        if (chuncked_position == - 1) {
            std::cout << "Chunked not found\n"; //TODO:: what to do in this case?
            _message_body_length = NOT_FOUND;
        }
        else if (chuncked_position == encodings_num - 1) {
            std::cout << "Will be handling chunks here\n";
            _message_body_length = CHUNCKED;
        }
        else {
            _throw_request_exception(HTTPResponse::BadRequest);
        }
    }

    void RequestParser::_define_message_body_length() {
        std::map<std::string, std::string> headers_map = _http_request_message->get_headers();
        std::map<std::string, std::string>::iterator transfer_encoding_iter = headers_map.find("Transfer-Encoding");
        std::map<std::string, std::string>::iterator content_length_iter = headers_map.find("Content-Length");
        if (content_length_iter != headers_map.end()) {
            if (transfer_encoding_iter == headers_map.end()) {
                _message_body_length = CONTENT_LENGTH;
            }
            else {
                _parse_transfer_encoding(transfer_encoding_iter->second);
                if (_message_body_length == CHUNCKED) {
                    _delete_obolete_content_length_header(); // if chunked is present Transfer-Encoding  overrides the Content-Length
                }
            }
        }
        else if (transfer_encoding_iter != headers_map.end()) {
            _parse_transfer_encoding(transfer_encoding_iter->second);
            if (_message_body_length != CHUNCKED) {
                _throw_request_exception(HTTPResponse::LengthRequired);
            }
        }
        else {
            _throw_request_exception(HTTPResponse::LengthRequired);
        }
    }

    void RequestParser::_parse_message_body(std::string& line) {
        _http_request_message->set_message_body(line);
        _current_parsing_state = FINISHED;
    }

    std::vector<std::string> RequestParser::_split_line(const std::string& line, const char delimiter){
        size_t start  = 0;
        std::vector<std::string> lines;

        while (true) {
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

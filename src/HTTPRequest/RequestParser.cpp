#include "RequestParser.hpp"
#include <algorithm> // for std::distance
#include <utility> // for std::make_pair
#include <cstdlib> // for atoi
#include <cctype> // for ::toupper
#include  <climits> // for INT_MAX

#include "HTTPRequestMethods.hpp"
#include "../HTTP/Exceptions/RequestException.hpp"
#include "../Utility/Utility.hpp"
#include "../Constants.hpp"


namespace HTTPRequest {

    RequestParser::Dispatch RequestParser::_dispatch_table[] = {
        {REQUEST_LINE, &RequestParser::_parse_request_line},
        {HEADER, &RequestParser::_parse_header},
        {PAYLOAD, &RequestParser::_parse_payload},
        {CHUNKED_PAYLOAD, &RequestParser::_decode_chunked},
        {FINISHED, NULL}
    };

    RequestParser::RequestParser(HTTPRequest::RequestMessage* http_request, HTTPResponse::ResponseMessage* http_response)
        : _current_parsing_state(REQUEST_LINE)
        , _http_request_message(http_request)
        , _http_response_message(http_response){}

    RequestParser::~RequestParser(){}

    void RequestParser::parse_HTTP_request(char* buffer, size_t bytes_read) {
        size_t content_length = 0;
        size_t bytes_accumulated = 0;
        while (bytes_accumulated != bytes_read || _current_parsing_state != FINISHED)
        {
            bool can_be_parsed = false;
            std::string line;
            if (_current_parsing_state == CHUNKED_PAYLOAD && _chunk_size > 0) {
                line = _request_reader.read_chunk(_chunk_size, buffer, bytes_read, &bytes_accumulated, &can_be_parsed);
            }
            else {
                line = _request_reader.read_line(buffer, bytes_read, &bytes_accumulated, &can_be_parsed);
            }
            if (_current_parsing_state == PAYLOAD && line.size() == content_length) {
                can_be_parsed = true;
            }
            // if the buffer is read and the request is complete( means the reader reached end of line for request line or header or read message body or message body size == 0)
            if (can_be_parsed == true) {
                _handle_request_message_part(line);
                if (_current_parsing_state == PAYLOAD) {
                    _define_message_body_length();
                    if (_payload_length == CONTENT_LENGTH) {
                        content_length = _set_content_length();
                    }
                    //TODO: validate request line
                    //TODO: validate headers
                }
            }
            else {
                return;
            }
        }
    }

    void RequestParser::_handle_request_message_part(std::string& line) {
        Dispatch *message = _dispatch_table;
        for (size_t i = 0; message[i].parsing_state != FINISHED; ++i) {
            if (message[i].parsing_state == _current_parsing_state) {
                (this->*(message[i].ptr))(line);
                return;
            }
        }
    }

    int RequestParser::_set_content_length() {
        std::string content_length_value = _http_request_message->get_headers().find("CONTENT_LENGTH")->second;
        if (content_length_value.find_first_of(',', 0) != std::string::npos) {
            std::vector<std::string> values = Utility::_split_line(content_length_value, ',');
            std::string first_value = Utility::_trim(values[0]);
            for (size_t i = 1; i < values.size(); ++i) {
                if (first_value != Utility::_trim(values[i])) {
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

    bool RequestParser::is_parsing_finished() {
        return _current_parsing_state == FINISHED;
    }

    void RequestParser::_throw_request_exception(HTTPResponse::StatusCode error_status) {
        _current_parsing_state = FINISHED;
        throw Exception::RequestException(error_status);
    }

    void RequestParser::_parse_request_line(std::string& line) {
        if (line.empty()) {

            _throw_request_exception(HTTPResponse::BadRequest);
        }
        std::vector<std::string> segments = Utility::_split_line(line, ' ');
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
            _current_parsing_state = PAYLOAD;
            return;
        }
        std::vector<std::string> segments = Utility::_split_line(line, ':');
        if (Utility::contains_whitespace(segments[0])) {
            _throw_request_exception(HTTPResponse::BadRequest);
        }
        std::string uppercased_header_name = _convert_header_name_touppercase(segments[0]);
        std::pair<std::string, std::string> header_field(uppercased_header_name, Utility::_trim(segments[1]));
        _http_request_message->set_header_field(header_field);
    }

    std::string RequestParser::_convert_header_name_touppercase(std::string& header_name) {
        std::transform(header_name.begin(), header_name.end(),header_name.begin(), ::toupper);
        std::replace_if (header_name.begin(), header_name.end(), Utility::is_hyphen, '_');
        return header_name;
    }

    void RequestParser::_define_message_body_length() {
        std::map<std::string, std::string> headers_map = _http_request_message->get_headers();
        std::map<std::string, std::string>::iterator transfer_encoding_iter = headers_map.find("TRANSFER_ENCODING");
        std::map<std::string, std::string>::iterator content_length_iter = headers_map.find("CONTENT_LENGTH");
        if (content_length_iter != headers_map.end()) { // if headers contain Content-Length
            if (transfer_encoding_iter == headers_map.end()) { // and headers don't contain Transfer-Encoding
                _payload_length = CONTENT_LENGTH;
            }
            else {
                _parse_transfer_encoding(transfer_encoding_iter->second);
                if (_payload_length == CHUNKED) {
                    _delete_obolete_content_length_header(); // TODO:: probably won't need this function // if chunked is present Transfer-Encoding  overrides the Content-Length
                    _current_parsing_state = CHUNKED_PAYLOAD;
                    _chunk_size = -1;
                    _decoded_body_length = 0;
                    _decoded_body = "";
                }
            }
        }
        else if (transfer_encoding_iter != headers_map.end()) { // if headers contain Transfer-Encoding without Content-length
            _parse_transfer_encoding(transfer_encoding_iter->second);
            if (_payload_length != CHUNKED) {
                _throw_request_exception(HTTPResponse::LengthRequired);
            }
        }
        else {
            if (_http_request_message->get_method() == "POST") {
                _throw_request_exception(HTTPResponse::LengthRequired);
            }
        }
    }


    // need to find the position of the 'chunked' in transfer-Encodeing as rfc demands to throw the 400 Error if 'chunked'is not the final encoding
    void RequestParser::_parse_transfer_encoding(std::string coding_names_list) {
        std::vector<std::string> encodings = Utility::_split_line(coding_names_list, ','); //splitting the header value as there might be multiple encodings
        ssize_t encodings_num = encodings.size();
        ssize_t chunked_position = _find_chunked_encoding_position(encodings, encodings_num);
        if (chunked_position == - 1) {
            std::cout << "Chunked not found\n"; //TODO:: what to do in this case?
            _payload_length = NOT_FOUND;
        }
        else if (chunked_position == encodings_num - 1) {
            std::cout << "Will be handling chunks here\n";
            _payload_length = CHUNKED;
        }
        else {
            _throw_request_exception(HTTPResponse::BadRequest);
        }
    }

    ssize_t RequestParser::_find_chunked_encoding_position(std::vector<std::string>& encodings, size_t encodings_num) {
        for (size_t i = 0; i < encodings_num; ++i) {
            if (Utility::_trim(encodings[i]) == "chunked") {
                return i;
            }
        }
        return -1;
    }

    void RequestParser::_delete_obolete_content_length_header() {
        _http_request_message->get_headers().erase("CONTENT_LENGTH");
    }

    void RequestParser::_parse_payload(std::string& line) {
        _http_request_message->set_payload(line);
        std::cout << "PAYLOAD: " << line << std::endl;
        _current_parsing_state = FINISHED;
    }

    void RequestParser::_decode_chunked(std::string& line) {
        if (_chunk_size == -1) { // it means we're dealing with the line defining the chunk_length
            _set_chunk_size(line);
            // TODO: should also read chunk extension
        }
        else {
            if (_chunk_size == 0) {
                // TODO: read_trailer_field() and add them to the headers;
                _assign_decoded_body_length_to_content_length();
                //TODO:: remove chunked from transfer encoding
                //TODO:: Remove Trailer from existing header fields
                _parse_payload(_decoded_body);
            }
            else {
                _decoded_body.append(line); // in this case we're dealing with the payload data
                size_t line_size = line.size();
                _chunk_size -= line_size;
                _decoded_body_length += line_size;
                if (_chunk_size == 0) {
                    _chunk_size = -1; // after handling the data we have to make sure we set the new chunk_size in the next iteration
                }
            }
        }
    }

    bool RequestParser::_is_last_chunk(size_t chunk_size) {
        return chunk_size == 0;
    }

    void RequestParser::_assign_decoded_body_length_to_content_length() {
        std::map<std::string, std::string> headers_map = _http_request_message->get_headers();
        const std::string content_length_header_name = "CONTENT_LENGTH";
        const std::string content_length_value = Utility::to_string(_decoded_body_length);
        std::map<std::string, std::string>::iterator content_length_iter = headers_map.find(content_length_header_name);
        if (content_length_iter != headers_map.end()) {
            std::pair<std::string, std::string> header_field(content_length_header_name, content_length_value);
            _http_request_message->set_header_field(header_field);
        }
        else {
            content_length_iter->second = content_length_value;
        }
    }

    void RequestParser::_set_chunk_size(std::string& line) {
        std::string extracted_number = Utility::get_number_in_string(line);
        if (extracted_number != "") {
            std::stringstream ss; // converting hex to the ssize_t
            ss << std::hex << extracted_number;
            ss >> _chunk_size;
            if (_chunk_size > Constants::PAYLOAD_MAX_LENGTH) {
                _throw_request_exception(HTTPResponse::BadRequest);
            }
        }
        else {
            _throw_request_exception(HTTPResponse::BadRequest);
        }
    }
}

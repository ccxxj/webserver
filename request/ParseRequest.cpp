#include "ParseRequest.hpp"
#include <cctype> // for isascii

// TODO: get this macro from config
#define MAX_SIZE_BODY 8000

ParseRequest::Dispatcher ParseRequest::dispatch_table[] = {
    {METHOD, ParseRequest::parse_method},
    {REQUEST_URI, ParseRequest::parse_request_uri},
    {HTTP_VERSION, ParseRequest::parse_HTTP_version},
    {HEADERS, ParseRequest::parse_headers},
    {MESSAGE_BODY, ParseRequest::parse_message_body},
};

// TODO: add check on the bytes read == -1 or == 0(if the client stopped the connection)
ParseRequest::ParseRequest(){}
ParseRequest::~ParseRequest(){}

bool ParseRequest::_is_end_of_request_line(char* current_string) {
    return *current_string == '\n';
}
bool ParseRequest::_is_end_of_header_fields(char* current_string, std::string& accumulating_string, size_t acc_length) {
    return accumulating_string[acc_length - 1] == '\r' && *current_string == '\n';
}

char* ParseRequest::_read_request_line(char* buffer, std::string& result, size_t* result_length) {
    while(!_is_end_of_request_line(buffer)) {
        if (*result_length < MAX_SIZE_BODY) {
            return; // TODO:: replace by error code
        }
        if (isascii(*buffer)) {
            result.append(1, *buffer);
            buffer++;
            result_length++;
        }
        else {
            std::cout << "Invalid character!" << std::endl; // TODO:: replace by error code
        }
    }
}

std::string& ParseRequest::read_HTTP_request(char* buffer, std::string& result) {
    size_t result_length = result.length();
    buffer = _read_request_line(buffer, result, &result_length);
    buffer = _read_header_fields(buffer, result, &result_length);
    while(!_is_end_of_header_fields(buffer, result, result_length) && result_length < MAX_SIZE_BODY) {
        if (isascii(*buffer)) {
            result.append(1, *buffer);
            buffer++;
            result_length++;
        }
    }
    std::cout << "body is: " << result << std::endl;
    return result;
}

#include "RequestReader.hpp"

#include "StatusCodes.hpp"

RequestReader::RequestReader() : _accumulator("") {}

RequestReader::~RequestReader()
{
}

std::string& RequestReader::get_accumulator() {
    return _accumulator;
}

bool RequestReader::_is_end_of_line(char* current_string) {
    return *current_string == '\n' && *(current_string - 1) == '\r';
}

bool RequestReader::_is_end_of_header_fields(char* current_string) {
    return *current_string == '\n' && *(current_string - 1) == '\r' && *(current_string - 2) == '\n' && *(current_string - 3) == '\r';
}

char* RequestReader::read_line(char* buffer, std::string& result) {
    while(!_is_end_of_line(buffer)) {
        if (RequestReader::_length_counter > MAX_SIZE_BODY) {
            return; // TODO:: replace by error code response
        }
        if (isascii(*buffer)) {
            result.append(1, *buffer);
            buffer++;
            RequestReader::_length_counter++;
        }
        else {
            std::cout << "Invalid character!" << std::endl; // TODO:: replace by returning error code
        }
    }
    return buffer;
}

std::string& RequestReader::read_HTTP_request(char* buffer, std::string& result) {
    buffer = _read_request_line(buffer, result);
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
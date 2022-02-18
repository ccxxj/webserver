#include "RequestReader.hpp"

#include "../HTTP/Exceptions/RequestException.hpp"

namespace HTTPRequest {

    size_t RequestReader::_length_counter = 0; // TODO create namespace

    RequestReader::RequestReader() : _accumulator(""){}

    RequestReader::~RequestReader()
    {
    }

    bool RequestReader::_is_end_of_line(char* buffer, char* buffer_end) { // also checking that the current char is part of the buffer
        return (buffer != buffer_end && *buffer == '\r'
                && buffer + 1 != buffer_end && *(buffer + 1) == '\n');
    }

    bool RequestReader::_is_end_of_header_fields(char* buffer, char* buffer_end) {
        return _is_end_of_line(buffer, buffer_end);
    }

    std::string RequestReader::read_line(char** buffer, char* buffer_end, bool* can_be_parsed) { // pointer to the buffer as we need to keep track of it
        char* temp_ptr = *buffer;
        while (temp_ptr != buffer_end)
        {
            if (RequestReader::_length_counter > MAX_SIZE_BODY) {
                throw Exception::RequestException(HTTPResponse::ContentTooLarge);
            }
            if (_is_end_of_line(temp_ptr, buffer_end)) {
                *can_be_parsed = true;
                std::string line = _accumulator.substr(0, (temp_ptr - *buffer));
                temp_ptr += 2; // skipping \r\n
                *buffer = temp_ptr;
                _accumulator.resize(0);
                return line;
            }
            if (isascii(*temp_ptr)) {
                _accumulator.append(1, *temp_ptr);
                temp_ptr++;
                RequestReader::_length_counter++;
            }
            else {
                throw Exception::RequestException(HTTPResponse::BadRequest);

            }
        }
        return _accumulator;
    }
}

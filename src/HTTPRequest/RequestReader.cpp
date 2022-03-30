#include "RequestReader.hpp"

#include "../HTTP/Exceptions/RequestException.hpp"

namespace HTTPRequest {

    size_t RequestReader::_length_counter = 0;

    RequestReader::RequestReader() : _accumulator("") {}

    RequestReader::~RequestReader() {}

    bool RequestReader::_is_end_of_line(char character) { // previous \r should already be a part of the accumulator
        return (character == '\n' && _accumulator.size() != 0 
                && _accumulator[_accumulator.size() - 1] == '\r');
    }

    bool RequestReader::_is_end_of_chunk(char* buffer, size_t bytes_accumulated) {
        return ((buffer + bytes_accumulated) && buffer[bytes_accumulated] == '\r' 
                && (buffer + bytes_accumulated + 1) && buffer[bytes_accumulated + 1] == '\n');
    }

    std::string RequestReader::read_line(char* buffer, size_t bytes_read, size_t* bytes_accumulated, bool* can_be_parsed) { // pointer to the buffer as we need to keep track of it
        while (*bytes_accumulated != bytes_read)
        {
            if (RequestReader::_length_counter > MAX_SIZE_BODY) {
                throw Exception::RequestException(HTTPResponse::ContentTooLarge);
            }
            char current_character = buffer[*bytes_accumulated];
            if (_is_end_of_line(current_character)) {
                *can_be_parsed = true;
                std::string line = _accumulator.substr(0, _accumulator.size() - 1); // -1 \r that has been appended, we don't want it to passed
                *bytes_accumulated += 1; // skipping \n
                _accumulator.resize(0);
                return line;
            }
            if (isascii(current_character)) {
                _accumulator.append(1, current_character);
                *bytes_accumulated += 1;
                RequestReader::_length_counter++;
            }
            else {
                std::cout << "ERROR REASON:  NON_ASCII\n";  // TODO: checking for ascii for request line and headers only?
                throw Exception::RequestException(HTTPResponse::BadRequest);

            }
        }
        return _accumulator;
    }

    std::string RequestReader::read_chunk(ssize_t chunk_size, char* buffer, size_t bytes_read, size_t* bytes_accumulated, bool* can_be_parsed) { // pointer to the buffer as we need to keep track of it
        while (*bytes_accumulated != bytes_read)
        {
            if (RequestReader::_length_counter > MAX_SIZE_BODY) {
                throw Exception::RequestException(HTTPResponse::ContentTooLarge);
            }
            char current_character = buffer[*bytes_accumulated];
            if (chunk_size > 0) {
                _accumulator.append(1, current_character);
                *bytes_accumulated += 1;
                RequestReader::_length_counter++;
                chunk_size--;
            }
            else {
                if (_is_end_of_chunk(buffer, *bytes_accumulated)) {
                    *can_be_parsed = true;
                    std::string line = _accumulator.substr(0, _accumulator.size());
                    *bytes_accumulated += 2; // skipping \r\n
                    _accumulator.resize(0);
                    return line;
                }
            }
        }
        return _accumulator;
    }
}

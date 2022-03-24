#include "RequestReader.hpp"

#include "../HTTP/Exceptions/RequestException.hpp"

namespace HTTPRequest {

    size_t RequestReader::_length_counter = 0;

    RequestReader::RequestReader() : _accumulator(""){}

    RequestReader::~RequestReader()
    {
    }

    bool RequestReader::_is_end_of_line(char character) { // previous \r should already be a part of the accumulator
        return (character == '\n' && *(_accumulator.rbegin()) == '\r');
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
                throw Exception::RequestException(HTTPResponse::BadRequest);

            }
        }
        return _accumulator;
    }

    std::string RequestReader::decode_chunked(char* buffer, size_t chunk_size, size_t bytes_read, size_t* bytes_accumulated, bool* can_be_parsed) { // pointer to the buffer as we need to keep track of it
        while (*bytes_accumulated != bytes_read)
        {
            if (RequestReader::_length_counter > MAX_SIZE_BODY) {
                throw Exception::RequestException(HTTPResponse::ContentTooLarge);
            }
            char current_character = buffer[*bytes_accumulated];
            if (_is_end_of_line(current_character)) {
                if (_is_last_chunk(chunk_size)) {
                    *can_be_parsed = true;
                }
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
                throw Exception::RequestException(HTTPResponse::BadRequest);

            }
        }
        return _accumulator;
    }

    bool RequestReader::_is_last_chunk(size_t chunk_size) {
        return chunk_size == 0;
    }
}
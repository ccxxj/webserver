#include "RequestReader.hpp"

#include "../HTTP/Exceptions/RequestException.hpp"

namespace HTTPRequest {

    size_t RequestReader::_length_counter = 0; // TODO create namespace

    RequestReader::RequestReader() : _accumulator(""){}

    RequestReader::~RequestReader()
    {
    }

    bool RequestReader::_is_end_of_line(char character) { // previous \r should already be a part of the accumulator
        return (character == '\n' && *(_accumulator.rbegin()) == '\r');
    }

    std::string RequestReader::read_line(char* buffer, size_t bytes_read, size_t* bytes_parsed, bool* can_be_parsed) { // pointer to the buffer as we need to keep track of it
        while (*bytes_parsed != bytes_read)
        {
            if (RequestReader::_length_counter > MAX_SIZE_BODY) {
                throw Exception::RequestException(HTTPResponse::ContentTooLarge);
            }
            char current_character = buffer[*bytes_parsed];
            if (_is_end_of_line(current_character)) {
                *can_be_parsed = true;
                std::string line = _accumulator.substr(0, _accumulator.size() - 1); // -1 \r that has been appended, we don't want it to passed
                *bytes_parsed += 1; // skipping \n
                _accumulator.resize(0);
                return line;
            }
            if (isascii(current_character)) {
                _accumulator.append(1, current_character);
                *bytes_parsed += 1;
                RequestReader::_length_counter++;
            }
            else {
                throw Exception::RequestException(HTTPResponse::BadRequest);

            }
        }
        return _accumulator;
    }
}

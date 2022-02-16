#include "RequestReader.hpp"

// #include "StatusCodes.hpp"

size_t RequestReader::_length_counter = 0; // TODO create namespace

RequestReader::RequestReader() {}

RequestReader::~RequestReader()
{
}

bool RequestReader::_is_end_of_line(char* buffer, char* message_end) { // also checking that the current char is part of the buffer
    return (buffer != message_end && *buffer == '\r'
            && buffer + 1 != message_end && *(buffer + 1) == '\n');
}

bool RequestReader::_is_end_of_header_fields(char* buffer, char* message_end) {
    return _is_end_of_line(buffer, message_end);
}

std::string RequestReader::read_line(char** buffer, char* message_end) { // pointer to the buffer as we need to keep track of it
    std::string accumulating_string = "";
    char* temp_ptr = *buffer;
    while (temp_ptr != message_end)
    {
        if (RequestReader::_length_counter > MAX_SIZE_BODY) {
            return ""; // TODO:: replace by error code response
        }
        if (_is_end_of_line(temp_ptr, message_end)) {
            temp_ptr += 2; // skipping \r\n
            break;
        }
        if (isascii(*temp_ptr)) {
            accumulating_string.append(1, *temp_ptr);
            temp_ptr++;
            RequestReader::_length_counter++;
        }
        else {
            std::cout << "Invalid character! num" << RequestReader::_length_counter << std::endl; // TODO:: replace by returning error code
            break;
        }
    }
    *buffer = temp_ptr;
    return accumulating_string;
}

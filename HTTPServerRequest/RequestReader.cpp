#include "RequestReader.hpp"

// #include "StatusCodes.hpp"

size_t RequestReader::_length_counter = 0; // TODO create namespace

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

bool RequestReader::_is_end_of_header_fields(char *current_string, std::string &accumulating_string) {
    return *current_string == '\n' && *(current_string - 1) == '\r' && *(current_string - 2) == '\n' && *(current_string - 3) == '\r';
}

char* RequestReader::read_line(char* buffer) {
    while(!_is_end_of_line(buffer)) {
        if (RequestReader::_length_counter > MAX_SIZE_BODY) {
            return NULL; // TODO:: replace by error code response
        }
        if (isascii(*buffer)) {
            _accumulator.append(1, *buffer);
            buffer++;
            RequestReader::_length_counter++;
        }
        else {
            std::cout << "Invalid character!" << std::endl; // TODO:: replace by returning error code
        }
    }
    return buffer;
}

#include "RequestParser.hpp"

// RequestParser::StateDispatcher RequestParser::dispatch_table[] = {
//     {METHOD, RequestParser::parse_method},
//     {REQUEST_URI, RequestParser::parse_request_uri},
//     {HTTP_VERSION, RequestParser::parse_HTTP_version},
//     {HEADERS, RequestParser::parse_headers},
//     {MESSAGE_BODY, RequestParser::parse_message_body},
// };

// TODO: add check on the bytes read == -1 or == 0(if the client stopped the connection)

RequestParser::RequestParser() {}

RequestParser::~RequestParser(){}

// TODO: replace with the state machine?
// TODO: check for any whitespaces which are not allowed
void RequestParser::parse_HTTP_request(char* buffer, size_t bytes_read) {
    char *message_end = buffer + bytes_read;
    std::string accumulator = request_reader.read_line(&buffer, message_end);
    _parse_request_line(accumulator);
    //TODO: validate request line
    while(buffer != message_end) {
        if (request_reader._is_end_of_header_fields(buffer, message_end)){
            buffer += 4; // skipping double \r\n
            break;
        }
        accumulator = request_reader.read_line(&buffer, message_end);
        _parse_header(accumulator);
    }
    //TODO: validate headers
}

std::vector<std::string> split_line(const std::string& line, const char delimiter){
    size_t start  = 0;
    std::vector<std::string> lines;

    while (true)
    {
        size_t match = line.find(delimiter, start);
        if (match == std::string::npos) {
            break; //TODO: fill response error
        }
        char ch = line[match];
        size_t len = match - start;
        lines.push_back(line.substr(start, len));
        start = match + 1;
    }
    lines.push_back(line.substr(start, line.size()));
    return lines;
}

//TODO: invalid request line results in 400 (Bad Request) response
void RequestParser::_parse_request_line(const std::string& accumulating_string) {
    if (accumulating_string == "")
        return; //TODO: fill error response
    std::vector<std::string> segments = split_line(accumulating_string, ' ');
    http_request_message.set_method(segments[0]);
    if (segments[1].size() > 2000) {
        return; //TODO: respond with 414 (URI Too Long) status code
    }
    http_request_message.set_request_uri(segments[1]);
    http_request_message.set_HTTP_version(segments[2]);
}

void RequestParser::_parse_header(const std::string& accumulating_string) {
        if (accumulating_string == "")
        return; //TODO: fill error response
    std::vector<std::string> segments = split_line(accumulating_string, ':');
    std::pair<std::string, std::string> header_field(segments[0], segments[1]); //TODO: No whitespace is allowed between the header field-name and colon.
    http_request_message.set_header_field(header_field); //TODO: what if the header name exists?
    // std::cout << "header name: " << segments[0] 
    //           << "      header value: " << segments[1] << std::endl;
}
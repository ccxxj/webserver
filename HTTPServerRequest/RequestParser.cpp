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
void RequestParser::parse_HTTP_request(char* buffer) {
    buffer = request_reader.read_line(buffer);
    _parse_request_line(request_reader.get_accumulator());
    // buffer = request_reader.read_line(buffer);
    // _parse_headers(request_reader.get_accumulator());
}


std::string RequestParser::_get_substring(size_t* start, const std::string& string) {
    const std::string delimiter = " ";
    size_t match = string.find(delimiter);
    if (match == std::string::npos) {
        return NULL; //TODO: fill response error
    }
    std::string substring = string.substr(*start, match);
    *start = match;
    return substring;
}

void RequestParser::_parse_request_line(const std::string& accumulating_string) {
    size_t start  = 0;
    std::string method = _get_substring(&start, accumulating_string);
    http_request_message.set_method(method);
    std::string request_uri = _get_substring(&start, accumulating_string);
    http_request_message.set_request_uri(request_uri);
    std::string HTTPversion = accumulating_string.substr(start, accumulating_string.size());
    http_request_message.set_HTTP_version(HTTPversion);
}
#include "RequestParser.hpp"
#include <cctype> // for isascii


RequestParser::StateDispatcher RequestParser::dispatch_table[] = {
    {METHOD, RequestParser::parse_method},
    {REQUEST_URI, RequestParser::parse_request_uri},
    {HTTP_VERSION, RequestParser::parse_HTTP_version},
    {HEADERS, RequestParser::parse_headers},
    {MESSAGE_BODY, RequestParser::parse_message_body},
};

// TODO: add check on the bytes read == -1 or == 0(if the client stopped the connection)
RequestParser::RequestParser(){}
RequestParser::~RequestParser(){}





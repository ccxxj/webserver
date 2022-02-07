#include "catch_amalgamated.hpp"

#include <string>
#include <fstream>

#include "../src/HTTP/RequestHandler.hpp"
#include "../src/HTTPRequest/RequestParser.hpp"
#include "../src/HTTPRequest/RequestMessage.hpp"
#include "../src/HTTPResponse/ResponseMessage.hpp"

namespace tests {

HTTPRequest::RequestMessage _http_request_message;
HTTPResponse::ResponseMessage _http_response_message;
HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);

char http_request_mes[] = "POST /cgi-bin/process.cgi HTTP/1.1\r\n"
                            "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
                            "Host: www.tutorialspoint.com\r\n"
                            "Content-Type: application/x-www-form-urlencoded\r\n"
                            "Content-Length: length\r\n"
                            "Accept-Language: en-us\r\n"
                            "Accept-Encoding: gzip, deflate\r\n"
                            "Connection: Keep-Alive\r\n\r\n"

                            "licenseID=string&content=string&/paramsXML=string";

TEST_CASE ("Simple generator use") {
    std::vector<char*> http_requests;
    std::string buffer;
    std::ifstream file("requestParserMessages.txt");
    if (file.is_open()) {
        std::string line;
        for( std::string line; getline(file, line); ) {
            while (line != "---end---") {
                buffer.append(line);
                buffer.append("\r\n");
            }
            char* buf = &buffer[0];
            http_requests.push_back(buf);
            getline(file, line); // skipping the newline after the end
        }
        file.close();
    }
        auto request = GENERATE(http_request_mes);
        parser.parse_HTTP_request(http_request_mes, strlen(http_request_mes));
        CHECK(_http_request_message.get_method() == "POST");
        CHECK(_http_request_message.get_request_uri() == "/cgi-bin/process.cgi");
        CHECK(_http_request_message.get_HTTP_version() == "HTTP/1.1");
    }
}
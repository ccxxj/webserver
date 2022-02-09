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

char http_request_mes[] = R"(POST /cgi-bin/process.cgi HTTP/1.1
                            User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)
                            Host: www.tutorialspoint.com
                            Content-Type: application/x-www-form-urlencoded
                            Content-Length: length
                            Accept-Language: en-us
                            Accept-Encoding: gzip, deflate
                            Connection: Keep-Alive

                            licenseID=string&content=string&/paramsXML=string
                            )";

    TEST_CASE ("Simple generator use") {
        std::vector<char*> http_requests;
        std::string buffer = "";

        std::ifstream file("/Users/osamara/codam_core/webserver/tests/requestParserMessages.txt");
        if (file.is_open()) {
            std::string line;
            while( file) {
                getline(file, line);
                if (line == "---end---") {
                    char* buf = &buffer[0];
                    http_requests.push_back(buf);
                    getline(file, line); // skipping the newline after the end
                }
                buffer.append(line);
                buffer.append("\r\n");
            
            }
            file.close();
        }
        // using IteratorType = std::vector<char*>::iterator;
        // std::vector<char*>::iterator it = http_requests.begin();
        // std::vector<char*>::iterator ite = http_requests.end();
        // auto request = GENERATE_REF(from_range(it, ite - 1));
        //  auto request = GENERATE_COPY(from_range(http_requests));

        // auto request = GENERATE(http_request_mes);

        parser.parse_HTTP_request(http_request_mes, strlen(http_request_mes));
        CHECK(_http_request_message.get_method() == "POST");
        CHECK(_http_request_message.get_request_uri() == "/cgi-bin/process.cgi");
        CHECK(_http_request_message.get_HTTP_version() == "HTTP/1.1");
    }
}

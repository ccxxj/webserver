#include "../catch_amalgamated.hpp"

#include <string>
#include <fstream>
#include <map>

#include "../../../src/HTTP/RequestHandler.hpp"

namespace tests {

    std::vector<std::string> http_requests;
    void fill_requests() {
        std::ifstream file("request_parser_unit_tests/request_parser_messages.txt");
        if (file.is_open()) {
            std::string line;
            std::string buffer = "";
            while( file) {
                getline(file, line);
                if (line == "------") {
                    if (buffer != "") {
                        http_requests.push_back(buffer);
                        buffer.resize(0);
                    }
                    getline(file, line); // skipping the line with index number, we don't need it
                }
                else {
                    buffer.append(line);
                    buffer.append("\r\n");
                }
            }
            file.close();
        }
    }

    TEST_CASE ("Parsing valid request messages") {
        fill_requests();
        HTTPRequest::RequestMessage _http_request_message;
        HTTPResponse::ResponseMessage _http_response_message;
        HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
        char* buf = &(http_requests[0])[0];
        parser.parse_HTTP_request(buf, strlen(buf));

            SECTION("Request line should be parsed and split into 3 atrributes") {
                CHECK(_http_request_message.get_method() == "POST");
                CHECK(_http_request_message.get_request_uri() == "/cgi-bin/process.cgi");
                CHECK(_http_request_message.get_HTTP_version() == "HTTP/1.1");
                
                    SECTION("Headers must be split by ':' and headers map must be filled") {
                        CHECK(_http_request_message.get_headers().size() == 7);
                        CHECK(_http_request_message.get_header_value("Accept-Language") == "en-us");
                    }

                    SECTION("Any number of spaces are allowed in header_values (after ''), they must be trimmed") {
                        CHECK(_http_request_message.get_header_value("Content-Type") == "application/x-www-form-urlencoded");
                    }
            }
    }
    TEST_CASE ("Parsing non_existing headers, map must be empty") {
        HTTPRequest::RequestMessage _http_request_message;
        HTTPResponse::ResponseMessage _http_response_message;
        HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
        char* buf = &(http_requests[7])[0];
        parser.parse_HTTP_request(buf, strlen(buf));    
        CHECK(_http_request_message.get_headers().size() == 0);            
    }
}

        // using IteratorType = std::vector<char*>::iterator;
        // std::vector<char*>::iterator it = http_requests.begin();
        // std::vector<char*>::iterator ite = http_requests.end();
        // auto request = GENERATE_REF(from_range(it, ite - 1));
        //  auto request = GENERATE_COPY(from_range(http_requests));
        // auto request = GENERATE(http_request_mes);
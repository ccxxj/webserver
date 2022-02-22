#include "../catch_amalgamated.hpp"

#include <string>
#include <fstream>
#include <map>

#include "../../../src/HTTP/RequestHandler.hpp"
#include "../../../src/HTTP/Exceptions/RequestException.hpp"

namespace tests {

    std::vector<std::string> fill_requests(const char* filename) {
    std::vector<std::string> http_requests;
        std::ifstream file(filename);
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
                    getline(file, line);
                    getline(file, line); // skipping 2 lines  with index number and description of the request, we don't need it
                }
                else {
                    buffer.append(line);
                    buffer.append("\r\n");
                }
            }
            file.close();
        }
        return http_requests;
    }

    TEST_CASE ("Request Parser", "[request_parser]") {
        std::vector<std::string> http_requests = fill_requests("request_parser_unit_tests/request_parser_messages.txt");
        SECTION ("Parsing valid request message", "[valid_request]") {

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

        SECTION ("Request with no headers passed, map is empty", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = &(http_requests[1])[0];
            parser.parse_HTTP_request(buf, strlen(buf));    
            CHECK(_http_request_message.get_headers().size() == 0);            

        }

    }

    TEST_CASE ("Invalid requests result in the response with error status codes", "[request_parser]") {
        std::vector<std::string> http_requests = fill_requests("request_parser_unit_tests/request_parser_messages_to_throw_exceptions.txt");
        SECTION ("Space between header field and colon not allowed, Bad Request must be thrown", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char *buf = &(http_requests[0])[0]; // header looks like this: "Host : localhost:80"
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
        }
    }
}


        // using IteratorType = std::vector<char*>::iterator;
        // std::vector<char*>::iterator it = http_requests.begin();
        // std::vector<char*>::iterator ite = http_requests.end();
        // auto request = GENERATE_REF(from_range(it, ite - 1));
        //  auto request = GENERATE_COPY(from_range(http_requests));
        // auto request = GENERATE(http_request_mes);
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
    
    char* create_writable_buf(std::string request) {
        std::string str = request;
        char* buf = new char[str.size() + 1];
        std::copy(str.begin(), str.end(), buf);
        buf[str.size()] = '\0'; // don't forget the terminating 0
        return buf;
    }

    TEST_CASE ("Request Parser - valid", "[request_parser]") {
        std::vector<std::string> http_requests = fill_requests("request_parser_unit_tests/request_parser_messages.txt");
        SECTION ("Parsing valid request message", "[valid_request]") {

            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            
            char* buf = create_writable_buf(http_requests[0]);
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
            delete[] buf;
        }

        SECTION ("Request with no headers passed, map is empty", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[1]);
            parser.parse_HTTP_request(buf, strlen(buf));
            CHECK(_http_request_message.get_headers().size() == 0);            
            delete[] buf;
        }

        SECTION ("Request multiple headers with the same name", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[2]);
            parser.parse_HTTP_request(buf, strlen(buf));  
            CHECK(_http_request_message.get_header_value("Accept") == "text/plain, text/html");
            delete[] buf;
        }
        // SECTION ("Request message coming with a delay", "[valid_request]") {

        //     HTTPRequest::RequestMessage _http_request_message;
        //     HTTPResponse::ResponseMessage _http_response_message;
        //     HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
        //     size_t index = 0;
        //     char* buf = create_writable_buf(http_requests[0]);
        //     size_t buf_size = http_requests[0].size();
        //     while (index < buf_size) {
        //         parser.parse_HTTP_request(buf, 1);
        //         index++;
        //     }
        //     SECTION("Request line should be parsed and split into 3 atrributes") {
        //         CHECK(_http_request_message.get_method() == "POST");
        //     }
        //     delete[] buf;
        // }
    }

    TEST_CASE ("Invalid requests - exceptions thrown", "[request_parser]") {
        std::vector<std::string> http_requests = fill_requests("request_parser_unit_tests/request_parser_messages_to_throw_exceptions.txt");
        SECTION ("Space between header field and colon not allowed, Bad Request must be thrown", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[0]); // header looks like this: "Host : localhost:80"
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
        }
        SECTION ("Spaces between request line elements not allowed, Bad Request must be thrown", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[1]); // P O ST /cgi-bin/process.cgi HTTP/1.1
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
        }
        SECTION ("Space inside uri, Bad Request must be thrown", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[2]); // POST /cgi-bin/ process.cgi HTTP/1.1
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
        }
        SECTION ("Spaces between request line and first header field are not allowed, Bad Request must be thrown", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[3]);
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
        }
        SECTION ("Invalid method in request line, Bad Request must be thrown", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[4]); //POSTT /foo.php HTTP/1.1
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
        }
        SECTION ("Only method in request line, Bad Request must be thrown", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[5]); // GET - without uri and version 
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
        }
        SECTION ("Method and http version in request line only, Bad Request must be thrown", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[6]); // GET HTTP/1.1 
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
        }
        SECTION ("Long uri in request line - LongUri 414", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[7]); // long uri
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
        }
    }

    // TEST_CASE ("Request Parser -delayed", "[request_parser]") {
    // std::vector<std::string> http_requests = fill_requests("request_parser_unit_tests/request_parser_messages.txt");
    //     SECTION ("Request message coming with a delay", "[valid_request]") {

    //         HTTPRequest::RequestMessage _http_request_message;
    //         HTTPResponse::ResponseMessage _http_response_message;
    //         HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
    //         char* buf = &(http_requests[0])[0];
    //         while (buf) {
    //             parser.parse_HTTP_request(buf, 1);
    //             buf++;
    //         }
    //         SECTION("Request line should be parsed and split into 3 atrributes") {
    //             CHECK(_http_request_message.get_method() == "POST");
    //             // CHECK(_http_request_message.get_request_uri() == "/cgi-bin/process.cgi");
    //             // CHECK(_http_request_message.get_HTTP_version() == "HTTP/1.1");
                
    //             //     SECTION("Headers must be split by ':' and headers map must be filled") {
    //             //         CHECK(_http_request_message.get_headers().size() == 7);
    //             //         CHECK(_http_request_message.get_header_value("Accept-Language") == "en-us");
    //             //     }

    //             //     SECTION("Any number of spaces are allowed in header_values (after ''), they must be trimmed") {
    //             //         CHECK(_http_request_message.get_header_value("Content-Type") == "application/x-www-form-urlencoded");
    //             //     }
    //         }
    //     }
    // }
}


        // using IteratorType = std::vector<char*>::iterator;
        // std::vector<char*>::iterator it = http_requests.begin();
        // std::vector<char*>::iterator ite = http_requests.end();
        // auto request = GENERATE_REF(from_range(it, ite - 1));
        //  auto request = GENERATE_COPY(from_range(http_requests));
        // auto request = GENERATE(http_request_mes);
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
                        CHECK(_http_request_message.get_header_value("ACCEPT_LANGUAGE") == "en-us");
                    }

                    SECTION("Any number of spaces are allowed in header_values (after ''), they must be trimmed") {
                        CHECK(_http_request_message.get_header_value("CONTENT_TYPE") == "application/x-www-form-urlencoded");
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
            CHECK(_http_request_message.get_header_value("ACCEPT") == "text/plain, text/html");
            delete[] buf;
        }

        SECTION ("Request message coming with a delay", "[valid_request]") {

            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            size_t index = 0;
            char* buf = create_writable_buf(http_requests[0]);
            size_t buf_size = http_requests[0].size();
            while (index < buf_size) {
                parser.parse_HTTP_request(buf + index, 1);
                index++;
            }
            SECTION("Request line should be parsed and split into 3 atrributes") {
                CHECK(_http_request_message.get_method() == "POST");
                CHECK(_http_request_message.get_request_uri() == "/cgi-bin/process.cgi");
                CHECK(_http_request_message.get_HTTP_version() == "HTTP/1.1");
                
                    SECTION("Headers must be split by ':' and headers map must be filled") {
                        CHECK(_http_request_message.get_headers().size() == 7);
                        CHECK(_http_request_message.get_header_value("ACCEPT_LANGUAGE") == "en-us");
                    }

                    SECTION("Any number of spaces are allowed in header_values (after ''), they must be trimmed") {
                        CHECK(_http_request_message.get_header_value("CONTENT_TYPE") == "application/x-www-form-urlencoded");
                    }
            }
            delete[] buf;
        }
        SECTION ("Multiple valid Content-length parameters with the same value: no exception is thrown", "[valid_request]") {

            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            
            char* buf = create_writable_buf(http_requests[3]);
            CHECK_NOTHROW(parser.parse_HTTP_request(buf, strlen(buf)));
            delete[] buf;
        }

        SECTION ("Transfer Encoding chunked", "[valid_request]") {

            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            
            char* buf = create_writable_buf(http_requests[5]);
            CHECK_NOTHROW(parser.parse_HTTP_request(buf, strlen(buf)));
            CHECK(_http_request_message.get_message_body() == "Wikipedia in\r\n\r\nchunks.");
            CHECK(_http_request_message.get_header_value("CONTENT_LENGTH") == "23");
            delete[] buf;
        }

        SECTION ("Transfer Encoding trailer after chunked payload", "[valid_request]") {

            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            
            char* buf = create_writable_buf(http_requests[6]);
            CHECK_NOTHROW(parser.parse_HTTP_request(buf, strlen(buf)));
            CHECK(_http_request_message.get_header_value("EXPIRES") == "Wed, 21 Oct 2015 07:28:00 GMT");
            delete[] buf;
        }
    }

    TEST_CASE ("Invalid requests - exceptions thrown", "[request_parser]") {
        std::vector<std::string> http_requests = fill_requests("request_parser_unit_tests/request_parser_messages_to_throw_exceptions.txt");
        SECTION ("Space between header field and colon not allowed, Bad Request must be thrown", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[0]); // header looks like this: "Host : localhost:80"
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
            delete[] buf;
        }
        SECTION ("Spaces between request line elements not allowed, Bad Request must be thrown", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[1]); // P O ST /cgi-bin/process.cgi HTTP/1.1
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
            delete[] buf;
        }
        SECTION ("Space inside uri, Bad Request must be thrown", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[2]); // POST /cgi-bin/ process.cgi HTTP/1.1
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
            delete[] buf;
        }
        SECTION ("Spaces between request line and first header field are not allowed, Bad Request must be thrown", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[3]);
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
            delete[] buf;
        }
        SECTION ("Invalid method in request line, Bad Request must be thrown", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[4]); //POSTT /foo.php HTTP/1.1
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
            delete[] buf;
        }
        SECTION ("Only method in request line, Bad Request must be thrown", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[5]); // GET - without uri and version 
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
            delete[] buf;
        }
        SECTION ("Method and http version in request line only, Bad Request must be thrown", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[6]); // GET HTTP/1.1 
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
            delete[] buf;
        }
        SECTION ("Long uri in request line - LongUri 414", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[7]); // long uri
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
            delete[] buf;
        }
        SECTION ("Negative Content-length - Bad Request must be thrown", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[8]); //-23 
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
            delete[] buf;
        }
        SECTION ("Multiple different Content-Length values - Bad Request must be thrown", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[9]); //23, 46, 23, 4
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
            delete[] buf;
        }
        SECTION ("Content-length values are the same but invalid - Bad Request must be thrown", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[10]); //23, 46, 23, 4
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
            delete[] buf;
        }
        SECTION ("No Content-Length or Transfer-Encoding but message body is there- 411 (Length Required) must be thrown", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[11]);
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
            delete[] buf;
        }

        SECTION ("Transfer-Encoding:  chunked is not the last encoding, should throw the 400 exception", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[12]);
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);
            delete[] buf;
        }

        SECTION ("Transfer-Encoding:  chunked with non-allowed trailer header field, should throw the 400 exception", "[invalid_request]") {
            HTTPRequest::RequestMessage _http_request_message;
            HTTPResponse::ResponseMessage _http_response_message;
            HTTPRequest::RequestParser parser(&_http_request_message, &_http_response_message);
            char* buf = create_writable_buf(http_requests[13]);
            
            CHECK_THROWS_AS((parser.parse_HTTP_request(buf, strlen(buf))), ::Exception::RequestException);

            delete[] buf;
        }
    }

}

#define CATCH_CONFIG_MAIN

#include "../catch_amalgamated.hpp"

#include <string>
#include <map>

#include "../../../src/HTTPRequest/URI/URIParser.hpp"
#include "../../../src/HTTPRequest/URI/URIData.hpp"
#include "../../../src/HTTP/Exceptions/RequestException.hpp"

namespace tests {
    TEST_CASE ("Parsing valid uri string", "[uri_parser]") {
        std::string uri_string;
        SECTION("empty uri input string"){
            uri_string = "";
            HTTPRequest::URIParser uri(uri_string);
            HTTPRequest::URIData uri_data;
            uri.parse(uri_data);
            CHECK(uri_data.get_path()[0] == "/");
        }
        SECTION("normal input string without query"){
            uri_string = "google/doc/asdfasdfasdfasdfasdfasdfasdfasdfasdf/ddddddddddddddd";
            HTTPRequest::URIParser uri(uri_string);
            HTTPRequest::URIData uri_data;
            uri.parse(uri_data);
            CHECK(uri_data.get_path()[0] == "google");
            CHECK(uri_data.get_path()[1] == "doc");
            CHECK(uri_data.get_path()[2] == "asdfasdfasdfasdfasdfasdfasdfasdfasdf");
            CHECK(uri_data.get_path()[3] == "ddddddddddddddd");
        }
        SECTION("normal input string with queries"){
            uri_string = "google/doc/asdfasdfasdfasdfasdfasdfasdfasdfasdf/ddddddddddddddd/?query1=abc&query2=ncd&aa=bb";
            HTTPRequest::URIParser uri(uri_string);
            HTTPRequest::URIData uri_data;
            uri.parse(uri_data);
            CHECK(uri_data.get_path()[0] == "google");
            CHECK(uri_data.get_path()[1] == "doc");
            CHECK(uri_data.get_path()[2] == "asdfasdfasdfasdfasdfasdfasdfasdfasdf");
            CHECK(uri_data.get_path()[3] == "ddddddddddddddd");
            CHECK(uri_data.get_query() == "query1=abc&query2=ncd&aa=bb");
        }
        SECTION("normal input string with queries and pct_encoding"){
            uri_string = "google/doc/asdfasdfasdfasdfasdfasdfasdfasdfasdf/ddddddddddddddd/?query%31=abc&query%32=ncd&a%61=bb";
            HTTPRequest::URIParser uri(uri_string);
            HTTPRequest::URIData uri_data;
            uri.parse(uri_data);
            CHECK(uri_data.get_path()[0] == "google");
            CHECK(uri_data.get_path()[1] == "doc");
            CHECK(uri_data.get_path()[2] == "asdfasdfasdfasdfasdfasdfasdfasdfasdf");
            CHECK(uri_data.get_path()[3] == "ddddddddddddddd");
            CHECK(uri_data.get_query() == "query1=abc&query2=ncd&aa=bb");
        }
        SECTION("normal input string with queries and pct_encoding and different hexdigit case sensative"){
            uri_string = "google/doc/asdfasdfasdfasdfasdfasdfasdfasdfasdf/ddddddddddddddd/?query%31=%6ebc&query%32=%6Ecd&a%61=bb";
            HTTPRequest::URIParser uri(uri_string);
            HTTPRequest::URIData uri_data;
            uri.parse(uri_data);
            CHECK(uri_data.get_path()[0] == "google");
            CHECK(uri_data.get_path()[1] == "doc");
            CHECK(uri_data.get_path()[2] == "asdfasdfasdfasdfasdfasdfasdfasdfasdf");
            CHECK(uri_data.get_path()[3] == "ddddddddddddddd");
            CHECK(uri_data.get_query() == "query1=nbc&query2=ncd&aa=bb");

        }
    }
    TEST_CASE ("Parsing invalid uri string", "[uri_parser]") {
        std::string uri_string;
        SECTION("test exception handling when pct_encoding is disqualified"){
            uri_string = "google/doc/?query%31=%6ebc&query%32=%6Ecd&a%6z=bb";
            HTTPRequest::URIParser uri(uri_string);
            HTTPRequest::URIData uri_data;
            CHECK_THROWS_AS(uri.parse(uri_data), Exception::RequestException);
        }

        //TODO check if this test is still valid
        // SECTION("test exception handling when missing euqal sign"){
        //     uri_string = "google/doc/?query%31=%6ebc&query%32=%6Ecd&a%65";
        //     HTTPRequest::URIParser uri(uri_string);
        //     HTTPRequest::URIData uri_data;
        //     CHECK_THROWS_AS(uri.parse(uri_data), Exception::RequestException);
        // }
    }
}

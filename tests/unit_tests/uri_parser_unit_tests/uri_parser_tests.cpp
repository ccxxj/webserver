#define CATCH_CONFIG_MAIN

#include "../catch_amalgamated.hpp"

#include <string>
#include <map>

#include "../../../src/URI/URIParser.hpp"
#include "../../../src/URI/URIData.hpp"
#include "../../../src/URI/Utils.hpp"


TEST_CASE ("Parsing valid uri string", "[uri_parser]") {
    std::string uri_string;
    SECTION("empty uri input string"){
        uri_string = "";
        URIParser uri(uri_string);
        uri.parse();
        CHECK(uri.get_uri_data().get_path()[0] == "/");
    }
    SECTION("normal input string without query"){
        uri_string = "google/doc/asdfasdfasdfasdfasdfasdfasdfasdfasdf/ddddddddddddddd";
        URIParser uri(uri_string);
        uri.parse();
        CHECK(uri.get_uri_data().get_path()[0] == "google");
        CHECK(uri.get_uri_data().get_path()[1] == "doc");
        CHECK(uri.get_uri_data().get_path()[2] == "asdfasdfasdfasdfasdfasdfasdfasdfasdf");
        CHECK(uri.get_uri_data().get_path()[3] == "ddddddddddddddd");
    }
    SECTION("normal input string with queries"){
        uri_string = "google/doc/asdfasdfasdfasdfasdfasdfasdfasdfasdf/ddddddddddddddd/?query1=abc&query2=ncd&aa=bb";
        URIParser uri(uri_string);
        uri.parse();
        std::map<std::string, std::string> query = uri.get_uri_data().get_query();
        CHECK(uri.get_uri_data().get_path()[0] == "google");
        CHECK(uri.get_uri_data().get_path()[1] == "doc");
        CHECK(uri.get_uri_data().get_path()[2] == "asdfasdfasdfasdfasdfasdfasdfasdfasdf");
        CHECK(uri.get_uri_data().get_path()[3] == "ddddddddddddddd");
        CHECK(query["query1"] == "abc");
        CHECK(query["query2"] == "ncd");
        CHECK(query["aa"] == "bb");
    }
    SECTION("normal input string with queries and pct_encoding"){
        uri_string = "google/doc/asdfasdfasdfasdfasdfasdfasdfasdfasdf/ddddddddddddddd/?query%31=abc&query%32=ncd&a%61=bb";
        URIParser uri(uri_string);
        uri.parse();
        std::map<std::string, std::string> query = uri.get_uri_data().get_query();
        CHECK(uri.get_uri_data().get_path()[0] == "google");
        CHECK(uri.get_uri_data().get_path()[1] == "doc");
        CHECK(uri.get_uri_data().get_path()[2] == "asdfasdfasdfasdfasdfasdfasdfasdfasdf");
        CHECK(uri.get_uri_data().get_path()[3] == "ddddddddddddddd");
        CHECK(query["query1"] == "abc");
        CHECK(query["query2"] == "ncd");
        CHECK(query["aa"] == "bb");
    }
    SECTION("normal input string with queries and pct_encoding and different hexdigit case sensative"){
        uri_string = "google/doc/asdfasdfasdfasdfasdfasdfasdfasdfasdf/ddddddddddddddd/?query%31=%6ebc&query%32=%6Ecd&a%61=bb";
        URIParser uri(uri_string);
        uri.parse();
        std::map<std::string, std::string> query = uri.get_uri_data().get_query();
        CHECK(uri.get_uri_data().get_path()[0] == "google");
        CHECK(uri.get_uri_data().get_path()[1] == "doc");
        CHECK(uri.get_uri_data().get_path()[2] == "asdfasdfasdfasdfasdfasdfasdfasdfasdf");
        CHECK(uri.get_uri_data().get_path()[3] == "ddddddddddddddd");
        CHECK(query["query1"] == "nbc");
        CHECK(query["query2"] == "ncd");
        CHECK(query["aa"] == "bb");
    }
}

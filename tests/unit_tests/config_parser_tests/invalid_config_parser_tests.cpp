#include "../catch_amalgamated.hpp"

#include <string>
#include <vector>

#include "../../../src/config/ConfigParser.hpp"
#include "../../../src/config/ConfigData.hpp"
#include "../../../src/config/AConfigBlock.hpp"
#include "../../../src/config/ServerBlock.hpp"
#include "../../../src/config/LocationBlock.hpp"
#include "../../../src/config/ConfigValidator.hpp"
#include "../../../src/config/ConfigTokenizer.hpp"

TEST_CASE("root directive check")
{
	SECTION("Multiple root lines")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/multiple_root_1");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("Multiple root on the same line")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/multiple_root_2");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
}

TEST_CASE("listen directive check")
{
	SECTION("Port is not a number: listen abc8080")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/invalid_port_num_1");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("Port is not a number: listen 8080abc;")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/invalid_port_num_3");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("Port num out of range")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/invalid_port_num_2");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("Invalid arg num: listen ;")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/invalid_port_num_4");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("Negative port num")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/invalid_port_num_5");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("More than 1 arg")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/invalid_port_num_6");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("Duplicate port ipv4")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/duplicate_port_2");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("Duplicate port ipv6")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/duplicate_port_1");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
}


TEST_CASE("Unknown directives")
{
	SECTION("Comments except #")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/invalid_directive_1");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("Unknown directive sbfvasfas")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/invalid_directive_2");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
}

TEST_CASE("return directive check")
{
	SECTION("More than 2 args: return 301 https://local 355;")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/invalid_return_1");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("No args: return ;")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/invalid_return_2");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}

	SECTION("invalid code: return 301abc https://localhost;")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/invalid_return_3");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("invalid code range: return -123 https://localhost;")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/invalid_return_4");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
}


TEST_CASE("client_max_body_size directive check")
{
	SECTION("duplicate directives")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/client_max_body_size_1");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("invalid value 120K")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/client_max_body_size_2");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("invalid size range")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/client_max_body_size_3");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("invalid num of args")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/client_max_body_size_4");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("No args")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/client_max_body_size_5");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
}

TEST_CASE("limit_except directive check")
{
	SECTION("no args")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/limit_except_1");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("invalid method")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/limit_except_2");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
}

TEST_CASE("duplicate location route")
{
	Config::ConfigValidator validator("config_parser_tests/conf_files/duplicate_location");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
}

TEST_CASE("error_page directive check")
{
	SECTION("no args")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/error_page_1");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("1 arg")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/error_page_2");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("more than 2 args - invalid value")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/error_page_3");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("negative invalid value")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/error_page_4");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("error code range between 300 - 599")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/error_page_5");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
}

TEST_CASE("server_name directive check")
{
		SECTION("no args")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/server_name_1");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
}

TEST_CASE("autoindex directive check")
{
	SECTION("no args")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/autoindex_2");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("more than 1 arg")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/autoindex_1");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
	SECTION("invalid arg")
	{
	Config::ConfigValidator validator("config_parser_tests/conf_files/autoindex_3");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	CHECK_THROWS(parser.parse());
	}
}
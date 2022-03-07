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

TEST_CASE("Empty conf")
{
	Config::ConfigValidator validator("data_check_after_parse/conf_files/empty_conf");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	parser.parse();
	CHECK_THROWS(config.check_parsed_data());
}

TEST_CASE("Missing listen line")
{
	Config::ConfigValidator validator("data_check_after_parse/conf_files/empty_conf");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	parser.parse();
	CHECK_THROWS(config.check_parsed_data());
}

TEST_CASE("Invalid port number")
{
	SECTION("Port is not a number")
	{
	Config::ConfigValidator validator("data_check_after_parse/conf_files/invalid_port_num_1");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	parser.parse();
	CHECK_THROWS(config.check_parsed_data());
	}
	SECTION("Port num out of range")
	{
	Config::ConfigValidator validator("data_check_after_parse/conf_files/invalid_port_num_2");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	parser.parse();
	CHECK_THROWS(config.check_parsed_data());
	}
}
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

TEST_CASE("Missing listen line - Default port:80")
{
	Config::ConfigValidator validator("data_check_after_parse/conf_files/no_listen");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigData config;
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	parser.parse();
	config.check_parsed_data();
	const std::vector<Config::ServerBlock> servers = config.get_servers();
	std::set<std::string> listen_set = servers[0].get_listen();
	std::set<std::string>::iterator i = listen_set.begin();
	CHECK(*i == "80");
}


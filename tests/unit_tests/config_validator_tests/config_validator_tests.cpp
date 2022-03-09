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

TEST_CASE("Unbalanced brackets")
{
	Config::ConfigValidator validator("config_validator_tests/conf_files/unbalanced_brackets");
	CHECK_THROWS(validator.validate());
}

TEST_CASE("Conf with different indentation")
{
	SECTION("Valid: no indentation")
	{
	Config::ConfigValidator validator("config_validator_tests/conf_files/unindented_conf");
	CHECK_NOTHROW(validator.validate());
	}
	SECTION("Valid: mix indentation")
	{
	Config::ConfigValidator validator2("config_validator_tests/conf_files/indented_conf");
	CHECK_NOTHROW(validator2.validate());
	}
}

TEST_CASE("Closing brackets")
{
	SECTION("Char after bracket: } asdsadsa")
	{
	Config::ConfigValidator validator1("config_validator_tests/conf_files/closing_bracket_1");
	CHECK_THROWS(validator1.validate());
	}
	SECTION("Char before bracket: asdsa}")
	{
	Config::ConfigValidator validator2("config_validator_tests/conf_files/closing_bracket_2");
	CHECK_THROWS(validator2.validate());
	}
}

TEST_CASE("Char outside of server blocks")
{
	SECTION("Before first server block")
	{
	Config::ConfigValidator validator1("config_validator_tests/conf_files/info_outside_block_1");
	CHECK_THROWS(validator1.validate());
	}
	SECTION("Inbetween server blocks")
	{
	Config::ConfigValidator validator2("config_validator_tests/conf_files//info_outside_block_2");
	CHECK_THROWS(validator2.validate());
	}
	SECTION("At the end of file")
	{
	Config::ConfigValidator validator3("config_validator_tests/conf_files//info_outside_block_3");
	CHECK_THROWS(validator3.validate());
	}
}

TEST_CASE("Missing semicolon")
{
	SECTION("Inside server block")
	{
	Config::ConfigValidator validator1("config_validator_tests/conf_files/missing_semicolon_1");
	CHECK_THROWS(validator1.validate());
	}
	SECTION("Inside location block")
	{
	Config::ConfigValidator validator2("config_validator_tests/conf_files//missing_semicolon_2");
	CHECK_THROWS(validator2.validate());
	}
	SECTION("Inside limit_except block")
	{
	Config::ConfigValidator validator3("config_validator_tests/conf_files//missing_semicolon_3");
	CHECK_THROWS(validator3.validate());
	}
	SECTION("anything after semicolon listen 100; abc;")
	{
	Config::ConfigValidator validator3("config_validator_tests/conf_files//missing_semicolon_4");
	CHECK_THROWS(validator3.validate());
	}
	SECTION("anything after semicolon listen 100; abc")
	{
	Config::ConfigValidator validator3("config_validator_tests/conf_files//missing_semicolon_5");
	CHECK_THROWS(validator3.validate());
	}
}

TEST_CASE("Server block opening line")
{
	SECTION("Text before server: asdasd server {")
	{
	Config::ConfigValidator validator1("config_validator_tests/conf_files/server_block_opening_1");
	CHECK_THROWS(validator1.validate());
	}
	SECTION("Valid: server{ & server		{")
	{
	Config::ConfigValidator validator2("config_validator_tests/conf_files/server_block_opening_2");
	CHECK_NOTHROW(validator2.validate());
	}
	SECTION("Anything except openning bracket: server{{}")
	{
	Config::ConfigValidator validator3("config_validator_tests/conf_files/server_block_opening_3");
	CHECK_THROWS(validator3.validate());
	}
}

TEST_CASE("Location block opening line")
{
	SECTION("No route: location {")
	{
	Config::ConfigValidator validator1("config_validator_tests/conf_files/location_block_opening_1");
	CHECK_THROWS(validator1.validate());
	}
	SECTION("Multiple routes: location /xyz /yxf {")
	{
	Config::ConfigValidator validator2("config_validator_tests/conf_files/location_block_opening_2");
	CHECK_THROWS(validator2.validate());
	}
	SECTION("Valid: location / { && location /{")
	{
	Config::ConfigValidator validator3("config_validator_tests/conf_files/location_block_opening_3");
	CHECK_NOTHROW(validator3.validate());
	}
	SECTION("Multiple routes: location /xyz /yxf {")
	{
	Config::ConfigValidator validator4("config_validator_tests/conf_files/location_block_opening_4");
	CHECK_THROWS(validator4.validate());
	}
	SECTION("Wrong keyword: locationabc /xyz {")
	{
	Config::ConfigValidator validator5("config_validator_tests/conf_files/location_block_opening_5");
	CHECK_THROWS(validator5.validate());
	}
}

TEST_CASE("limit_except directive")
{
	SECTION("inside server block")
	{
	Config::ConfigValidator validator1("config_validator_tests/conf_files/limit_except_1");
	CHECK_THROWS(validator1.validate());
	}
	SECTION("unknown directive in limit_except")
	{
	Config::ConfigValidator validator1("config_validator_tests/conf_files/limit_except_2");
	CHECK_THROWS(validator1.validate());
	}
	SECTION("unknown directive limit_exceptasd")
	{
	Config::ConfigValidator validator1("config_validator_tests/conf_files/limit_except_3");
	CHECK_THROWS(validator1.validate());
	}
	SECTION("wrong deny arg")
	{
	Config::ConfigValidator validator1("config_validator_tests/conf_files/limit_except_4");
	CHECK_THROWS(validator1.validate());
	}
	SECTION("invalid number of arguments in deny directive")
	{
	Config::ConfigValidator validator1("config_validator_tests/conf_files/limit_except_5");
	CHECK_THROWS(validator1.validate());
	}
	SECTION("unknown directive denyxx all;")
	{
	Config::ConfigValidator validator1("config_validator_tests/conf_files/limit_except_6");
	CHECK_THROWS(validator1.validate());
	}
}
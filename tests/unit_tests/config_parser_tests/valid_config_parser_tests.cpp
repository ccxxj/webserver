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

TEST_CASE("Parsing basic conf file")
{
	Config::ConfigData config;
	Config::ConfigValidator validator("config_parser_tests/conf_files/basic-conf");
	validator.validate();
	Config::ConfigTokenizer tokenizer(validator.get_file_content());
	tokenizer.tokenize_server_blocks();
	Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
	parser.parse();

	SECTION("Conf file has 4 server blocks, so ConfigData should have 4 servers")
	{
		std::vector<Config::ServerBlock> servers = config.get_servers();
		CHECK(servers.size() == 4);
		SECTION("1st server block checks: should be default, listen, server_name, root, body_size, return")
		{
			CHECK(servers[0].get_default() == true);
			// CHECK(servers[0].get_listen().size() == 1);
			// CHECK(servers[0].get_listen()[0] == "1000");
			CHECK(servers[0].get_server_name().size() == 2);
			CHECK(servers[0].get_server_name()[0] == "irem");
			CHECK(servers[0].get_server_name()[1] == "webservvvvv");
			CHECK(servers[0].get_root() == "/var/www/localhost");
			CHECK(servers[0].get_client_max_body_size() == 200);
			CHECK(servers[0].get_error_page().size() == 3);
			CHECK(servers[0].get_error_page()[0] == "404");
			CHECK(servers[0].get_error_page()[1] == "405");
			CHECK(servers[0].get_error_page()[2] == "/custom-404.html");
			CHECK(servers[0].get_return().size() == 2);
			CHECK(servers[0].get_return()[0] == "301");
			CHECK(servers[0].get_return()[1] == "https://$server_name$request_uri");
		}

		SECTION("2nd server block checks: not default, locations, comments")
		{
			CHECK(servers[1].get_default() == false);
			// CHECK(servers[1].get_listen().size() == 1);
			// CHECK(servers[1].get_listen()[0] == "2000");
			CHECK(servers[1].get_server_name().size() == 1);
			CHECK(servers[1].get_server_name()[0] == "random_name");
			CHECK(servers[1].get_root() == "/var/www/localhost");
			CHECK(servers[1].get_client_max_body_size() == 1);
			CHECK(servers[1].get_error_page().size() == 2);
			CHECK(servers[1].get_error_page()[0] == "404");
			CHECK(servers[1].get_error_page()[1] == "/custom-404.html");
			SECTION("Location blocks in 2nd server checks") //TODO scope with locs inside sections, separate them in diff sections?
			{
				std::vector<Config::LocationBlock> locs = servers[1].get_location();
				CHECK(locs.size() == 3);
				//1st loc
				CHECK(locs[0].get_route() == "/wordpress/");
				CHECK(locs[0].get_root() == "/var/www/localhost");
				CHECK(locs[0].get_error_page().size() == 2);
				CHECK(locs[0].get_error_page()[0] == "301");
				CHECK(locs[0].get_error_page()[1] == "/custom-301.html");
				CHECK(locs[0].get_autoindex() == 1); //ON
				CHECK(locs[0].get_limit_except().size() == 1);
				CHECK(locs[0].get_limit_except()[0] == "POST");
				CHECK(locs[0].get_return().size() == 2);
				CHECK(locs[0].get_return()[0] == "301");
				CHECK(locs[0].get_return()[1] == "https://localhost");
				//2nd loc
				CHECK(locs[1].get_route() == "/myyy_route/");
				CHECK(locs[1].get_autoindex() == 0); //OFF
				CHECK(locs[1].get_error_page().size() == 2);
				CHECK(locs[1].get_error_page()[0] == "301");
				CHECK(locs[1].get_error_page()[1] == "/custom-301.html");
				CHECK(locs[1].get_return().size() == 2);
				CHECK(locs[1].get_return()[0] == "301");
				CHECK(locs[1].get_return()[1] == "https://localhost");
				//3rd loc
				CHECK(locs[2].get_route() == "/whatssssup/");
				CHECK(locs[2].get_autoindex() == 1); //
				CHECK(locs[2].get_root() == "/var/www/iremiremirem");
				CHECK(locs[2].get_return().size() == 2);
				CHECK(locs[2].get_return()[0] == "505");
				CHECK(locs[2].get_return()[1] == "https://$server_name$request_uri");
			}
		}

	SECTION("3rd server block checks: not default, listen, body_size")
	{
		CHECK(servers[2].get_default() == false);
		// CHECK(servers[2].get_listen().size() == 1);
		// CHECK(servers[2].get_listen()[0] == "8080");
		CHECK(servers[2].get_server_name().size() == 1);
		CHECK(servers[2].get_server_name()[0] == "third_server");
		CHECK(servers[2].get_client_max_body_size() == 200);
	}
	SECTION("4th server block checks: not default, listen, body_size")
	{
		CHECK(servers[3].get_default() == false);
		// CHECK(servers[3].get_listen().size() == 1);
		// CHECK(servers[3].get_listen()[0] == "2020");
		CHECK(servers[3].get_server_name().size() == 1);
		CHECK(servers[3].get_server_name()[0] == "fourth_server");
		CHECK(servers[3].get_client_max_body_size() == 500);
	}
	}
}

#include "ConfigParser.hpp"
#include <stdlib.h>
#include "../Utility/Utility.hpp"

namespace Config
{

	ConfigParser::ConfigParser(ConfigData *config_data, std::vector<std::string> server_tokens) : config_data(config_data),
																								   server_tokens(server_tokens)
	{
	}

	ConfigParser::~ConfigParser()
	{
	}

	bool ConfigParser::find_location(std::string line)
	{
		return Utility::check_first_keyword(line, "location");
	}

	int ConfigParser::find_directive(std::string& line)
	{
		const char *directive_list[11] =
			{"listen", "server_name", "client_max_body_size",
			 "error_page", "return", "root", "limit_except",
			 "autoindex", "location", "ext", NULL};
		for (size_t i = 0; i < 10; i++)
		{
			if (Utility::check_first_keyword(line, directive_list[i]))
				return i;
		}
		return -1;
	}

	void ConfigParser::parse_limit_except(std::string& line, LocationBlock &location, std::istringstream &stream)
	{
		location.set_limit_except(line);
		while (std::getline(stream, line))
		{
			std::string temp = line;
			Utility::remove_white_space(temp);
			if (temp.compare("}") == 0)
				break;
			else
				continue;
		}
	}

	void ConfigParser::parse_location_block(std::string& line, std::istringstream &stream, ServerBlock &server)
	{
		LocationBlock location;
		int e_num = -4;
		e_num = find_directive(line);
		parse_location_directive(line, location, e_num);
		while (std::getline(stream, line))
		{
			std::string temp = line;
			Utility::remove_white_space(temp);
			if (temp.compare("}") == 0)
				break;
			if (Utility::check_first_keyword(line, "limit_except"))
				parse_limit_except(line, location, stream);
			else if ((e_num = find_directive(line)) >= 0)
				parse_location_directive(line, location, e_num);
			else
				throw std::runtime_error("unknown directive " + line);
		}
		server.set_a_location(location);
	}

	void ConfigParser::parse_server_directive(std::string& line, ServerBlock &server, int e_num)
	{
		if (e_num == LISTEN)
			server.set_listen(line);
		else if (e_num == SERVER_NAME)
			server.set_server_name(line);
		else if (e_num == BODY_SIZE)
			server.set_client_max_body_size(line);
		else if (e_num == ERROR_PAGE)
			server.set_error_page_value(line);
		else if (e_num == RETURN)
			server.set_return_value(line);
		else if (e_num == ROOT)
			server.set_root_value(line);
		else if (e_num == EXT)
			server.set_extention_list(line);
		//TODO anything specific to valid location block directive outside of server context
		// else
		// 	throw std::runtime_error("unknown directive " + line);

	}

	void ConfigParser::parse_location_directive(std::string& line, LocationBlock &location, int e_num)
	{
		if (e_num == ROOT)
			location.set_root_value(line);
		else if (e_num == ERROR_PAGE)
			location.set_error_page_value(line);
		else if (e_num == BODY_SIZE)
			location.set_client_max_body_size(line);
		else if (e_num == ROUTE)
			location.set_route(line);
		else if (e_num == RETURN)
			location.set_return_value(line);
		else if (e_num == LIMIT_EXCEPT)
			location.set_limit_except(line);
		else if (e_num == AUTOINDEX)
			location.set_autoindex(line);
		else
			throw std::runtime_error("unknown directive " + line);
	}

	void ConfigParser::parse_server_block(std::string& server_token, ServerBlock &server)
	{
		std::string line;
		std::istringstream stream(server_token);
		int e_num;

		while (std::getline(stream, line))
		{
			if (ConfigParser::find_location(line))
				parse_location_block(line, stream, server);
			else if ((e_num = find_directive(line)) >= 0)
				parse_server_directive(line, server, e_num);
			else
				throw std::runtime_error("unknown directive " + line);
		}
	}

	void ConfigParser::parse(void)
	{
		for (size_t i = 0; i < server_tokens.size(); i++)
		{
			ServerBlock server;
			parse_server_block(server_tokens[i], server);
			server.set_id(i + 1);
			config_data->set_a_server(server);
		}
		config_data->make_first_server_default();
	}
} // namespace Config

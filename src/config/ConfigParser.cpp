#include "ConfigParser.hpp"
#include <stdlib.h>
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
		return Utils::check_first_keyword(line, "location");
	}

	int ConfigParser::find_directive(std::string line)
	{
		const char *directive_list[10] =
			{"listen", "server_name", "client_max_body_size",
			 "error_page", "return", "root", "limit_except",
			 "autoindex", "location", NULL};
		for (size_t i = 0; i < 9; i++)
		{
			if (Utils::check_first_keyword(line, directive_list[i]))
				return i;
		}
		return -1;
	}

	void parse_limit_except(std::string line, LocationBlock &location, std::istringstream &stream)
	{
		location.set_limit_except(line);
		while (std::getline(stream, line))
		{
			std::string temp = line;
			Utils::remove_white_space(temp);
			if (temp.compare("}") == 0)
				break;
			else
				continue;
		}
	}

	void ConfigParser::parse_location_block(std::string line, std::istringstream &stream, ServerBlock &server)
	{
		//create the location + push back into to the server.getLocation().push_back();
		LocationBlock location;
		int e_num = -4;
		e_num = find_directive(line);
		parse_location_directive(line, location, e_num);
		while (std::getline(stream, line))
		{
			std::string temp = line;
			Utils::remove_white_space(temp);
			if (temp.compare("}") == 0)
				break;
			if (Utils::check_first_keyword(line, "limit_except"))
				parse_limit_except(line, location, stream);
			else if ((e_num = find_directive(line)) >= 0)
				parse_location_directive(line, location, e_num);
			else
				throw ConfigException("Invalid directive in config");
		}
		server.get_location().push_back(location);
	}

	void ConfigParser::parse_server_directive(std::string line, ServerBlock &server, int e_num)
	{
		//identify the keyword, remove keyword and use the right set functoion
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
	}

	void ConfigParser::parse_location_directive(std::string line, LocationBlock &location, int e_num)
	{
		//identify the keyword, remove keyword and use the right set functoion
		if (e_num == ROOT)
			location.set_root_value(line);
		else if (e_num == ERROR_PAGE)
			location.set_error_page_value(line);
		else if (e_num == ROOT)
			location.set_root_value(line);
		else if (e_num == ROUTE)
			location.set_route(line);
		else if (e_num == RETURN)
			location.set_return_value(line);
		else if (e_num == LIMIT_EXCEPT)
			location.set_limit_except(line);
		else if (e_num == AUTOINDEX)
			location.set_autoindex(line);
	}

	void ConfigParser::parse_server_block(std::string server_token, ServerBlock &server)
	{
		// loop inside string find key value pairs & location values
		// saves them inside the variables of ServerBlock and LocationBlocks!
		// call ServerBlock & LocationBlock functions based on the line?
		// what to do with compulsory fields? throw exception!
		std::string line;
		std::istringstream stream(server_token);
		int e_num = -4; //TODO
		while (std::getline(stream, line))
		{
			if (ConfigParser::find_location(line))
				parse_location_block(line, stream, server);
			else if ((e_num = find_directive(line)) >= 0)
				parse_server_directive(line, server, e_num);
			else
				throw ConfigException("Invalid directive in config");
		}
	}

	void ConfigParser::parse(void)
	{
		for (size_t i = 0; i < server_tokens.size(); i++)
		{
			ServerBlock server;
			parse_server_block(server_tokens[i], server);
			config_data->get_servers().push_back(server);
		}
		config_data->make_first_server_default();
	}
	//TODO after parse checks: empty file, no server block, compulsory fields: listen
	//TODO detailed after parse checks: i.e. multiple roots  on the same line or multiple lines, return line with more than 2 info (check your list for more)
} // namespace Config

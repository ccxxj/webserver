#include "ConfigParser.hpp"
#include <stdlib.h>
namespace Config
{

	ConfigParser::ConfigParser(ConfigData *config_data, std::string file_path) : config_data(config_data),
																				 file_path(file_path)
	{
	}

	ConfigParser::~ConfigParser()
	{
	}

	void ConfigParser::open_and_read_file(void)
	{
		std::ifstream file_stream;
		std::string line;

		file_stream.open(file_path);
		if (!file_stream.is_open())
			throw ConfigParser::FailedToOpenException();
		while (std::getline(file_stream, line))
			file_content.append(line + "\n");
		file_stream.close();
	}

	void ConfigParser::remove_comments(void)
	{
		std::istringstream stream(file_content);
		std::string line;
		size_t hashtag_pos;

		file_content.clear();
		while (std::getline(stream, line))
		{
			hashtag_pos = line.find('#');
			if (hashtag_pos != std::string::npos)
				line.erase(hashtag_pos, std::string::npos);
			if (!line.empty())
				file_content.append(line + "\n");
		}
	}

	void ConfigParser::tokenize_server_blocks(void)
	{
		std::string line;
		std::string single_server_block;
		std::istringstream stream(file_content); //TODO do we check if it is streamed? if (!stream)

		while (std::getline(stream, line))
		{
			if (line.compare("}") == 0) //TODO it depends on indenentation rules! test with unindented congfigs
			{
				server_tokens.push_back(single_server_block);
				single_server_block.clear();
			}
			else if (line.compare("server {") != 0)
				single_server_block.append(line + "\n");
		}
	}

	void ConfigParser::print_server_blocks(void)
	{

		for (size_t i = 0; i < server_tokens.size(); i++)
		{
			std::cout << "\nEACH SERVER BLOCK" << std::endl;
			std::cout << server_tokens[i] << std::endl;
		}
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
			else if (line.compare("\t") == 0) //TODO = I think this is shaky? needs more testing? this is for empty lines (that only have tabs after removing the comments, so anything else to add
				continue;
			else
				throw ConfigParser::InvalidConfigDirectiveException();
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
			else if (line.compare("\t") == 0) //TODO this is for empty lines (that only have tabs after removing the comments, so anything else to add
				continue;
			else
				throw ConfigParser::InvalidConfigDirectiveException();
		}
	}

	//TODO it still parses without ; at the end of lines!
	void ConfigParser::parse(void)
	{
		open_and_read_file();
		remove_comments();
		tokenize_server_blocks();
		// print_server_blocks();
		for (size_t i = 0; i < server_tokens.size(); i++)
		{
			ServerBlock server;
			parse_server_block(server_tokens[i], server);
			config_data->get_servers().push_back(server);
		}
		config_data->make_first_server_default();
	}

} // namespace Config

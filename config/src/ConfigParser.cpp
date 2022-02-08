#include "ConfigParser.hpp"
#include <stdlib.h> 
// namespace Config {

ConfigParser::ConfigParser(ConfigData *config_data, std::string file_path):
	config_data(config_data),
	file_path(file_path)
{
	
}

// ConfigParser::ConfigParser(std::string file_path): file_path(file_path)
// {
// }

ConfigParser::~ConfigParser()
{
}

void	ConfigParser::open_and_read_file( void )
{
	std::ifstream file_stream;
	std::string line;

	file_stream.open(file_path);
	if (!file_stream.is_open())
		throw ConfigParser::FailedToOpenException();
	while (std::getline(file_stream, line))
	{
		file_content.append(line);
		file_content.append("\n");
	}
	file_stream.close();
}

void	ConfigParser::remove_comments( void )
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
		//TODO some lines still have tabs and they are saved into the file_content. Should I remove them?
		if(!line.empty())
		{
			// std::cout << line << std::endl;
			file_content.append(line);
			file_content.append("\n");
		}
	}
}

//TODO remove the first line and last line server { }  from the string?
void	ConfigParser::tokenize_server_blocks( void )
{
	std::string line;
	std::string single_server_block;
	std::istringstream stream(file_content); //TODO do we check if it is streamed?

	while (std::getline(stream, line))
	{
		single_server_block.append(line);
		if (line.compare("}") == 0)
		{
			server_tokens.push_back(single_server_block);
			single_server_block.clear();
		}
		else
			single_server_block.append("\n");
	}
}

void	ConfigParser::print_server_blocks( void )
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

bool ConfigParser::find_directive(std::string line)
{
	const char *directive_list[8] = {"listen", "server_name", "client_max_body_size", "error_page", "return", "root", "limit_except", NULL};
	//auto index and directory listing?
	for (size_t i = 0; i < 7; i++)
	{
		if(Utils::check_first_keyword(line, directive_list[i]))
			return true;
	}
	return false;
}

void ConfigParser::parse_location_block(std::string line, std::istringstream &stream)
{
	LocationBlock location;
	//call set_route on the line (but first get rid of location {})
	// std::cout << "in parse location" << std::endl;
	while (std::getline(stream, line))
	{
		// std::cout << line << std::endl;
		//when you hav limit except you need to skip oher lines?
	//IGNORE THE LAST LINE
	//STOP AT THE END OF BRACKET
	}

}
//TODO very shitty parameters, check if reference should be passed!
void ConfigParser::parse_server_block(std::string server_token, ServerBlock &server)
{
	std::cout << "in parse server" << std::endl;
	// ServerBlock server;
	
	// loop inside string find key value pairs & location values
	// saves them inside the variables of ServerBlock and LocationBlocks!
	//read line by line? call ServerBlocks functions based on the line?
	//what to do with compulsory fields? throw exception!

	std::string line;
	std::istringstream stream(server_token);
	// int x = rand();
	// server.set_server_name(std::to_string(x) + " ;");
	server.set_server_name(" blablabla ;");
	// server.get_server_name();
	while (std::getline(stream, line))
	{
		// std::cout << line << std::endl;
		// std::cout << "X: " << Utils::check_first_keyword(line, "listen") << std::endl;
		// std::cout << "/* message */" << std::endl;
		//create the location + push back into to the server.getLocation().push_back();
		if(ConfigParser::find_location(line))
			parse_location_block(line, stream); //record the route, limit_except? root,
		else if(ConfigParser::find_directive(line))
			parse_directive(); //identify the keyword, remove keyword and use the right set functoion
		//	
		// else
		// 	throw std::exception;//TODO handle exception
	}
	// return(server);
	std::cout << "end of parse server" << std::endl;
}

void	ConfigParser::parse( void )
{
	std::cout << "IN PARSE" << std::endl;
	(void)config_data;
	open_and_read_file();
	remove_comments();
	tokenize_server_blocks();
	// print_server_blocks();

	// std::vector<ServerBlock> _servers = ; 
	for (size_t i = 0; i < server_tokens.size(); i++)
	{
		// std::cout << "my tokenss " << server_tokens[i] << std::endl;
		std::cout << "i: " << i << std::endl;
		ServerBlock server;
		parse_server_block(server_tokens[i], server);
		config_data->get_servers().push_back(server);
		
	}
	std::cout << "last line off parse" << std::endl;
	// std::cout << "size of servers vector in ConfigData " << _servers.size() << std::endl;		
}

// }

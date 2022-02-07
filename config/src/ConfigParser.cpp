#include "ConfigParser.hpp"

namespace Config {

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
			std::cout << line << std::endl;
			file_content.append(line);
			file_content.append("\n");
		}
	}
}

void	ConfigParser::tokenize_server_blocks( void )
{
	std::string line;
	std::string single_server_block;
	std::istringstream stream(file_content);

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

//TODO very shitty parameters, check if reference should be passed!
ServerBlock &ConfigParser::parse_server_block(std::string server_token)
{
	ServerBlock server;
	(void)server_token;
	

	// loop inside string find key value pairs & location values
	// saves them inside the variables of ServerBlock and LocationBlocks!
	return(server);
}

void	ConfigParser::parse( void )
{
	(void)config_data;
	open_and_read_file();
	remove_comments();
	tokenize_server_blocks();
	print_server_blocks();

	std::vector<ServerBlock> _servers = config_data->get_servers();
	for (size_t i = 0; i < server_tokens.size(); i++)
		_servers.push_back(parse_server_block(server_tokens[i]));
}

}

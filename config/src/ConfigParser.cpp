#include "ConfigParser.hpp"

ConfigParser::ConfigParser(std::string file_path): file_path(file_path)
{
	// TODO not sure it's good practice to open and read in constructor.
	file_stream.open(this->file_path);
	if (!file_stream.is_open())
		throw ConfigParser::FailedToOpenException();
	tokenize_server_blocks();
	// std::cout << file_content << std::endl;
	file_stream.close();
}

ConfigParser::~ConfigParser()
{
}

void	ConfigParser::tokenize_server_blocks( void )
{
	std::string	line;
	std::string	single_server_block;

	while (std::getline(file_stream, line))
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
	print_server_blocks();
}

void	ConfigParser::print_server_blocks( void )
{

	for (size_t i = 0; i < server_tokens.size(); i++)
	{
		std::cout << "\nEACH SERVER BLOCK TOKEN" << std::endl;
		std::cout << server_tokens[i] << std::endl;
	}
}

void	ConfigParser::parse( void )
{
	// std::cout << "in parse" << std::endl;
}

#include "ConfigTokenizer.hpp"

namespace Config
{

	ConfigTokenizer::ConfigTokenizer(std::string file_content) :_file_content(file_content)
	{
	}

	ConfigTokenizer::~ConfigTokenizer()
	{
	}

	void ConfigTokenizer::tokenize_server_blocks(void)
	{
		std::string line;
		std::string single_server_block;
		std::istringstream stream(_file_content);

		while (std::getline(stream, line))
		{
			if (line.find("location") != std::string::npos)
				_tokenize_location_block(line, stream, single_server_block);
			else if (line.find("}") != std::string::npos)
			{
				_server_tokens.push_back(single_server_block);
				single_server_block.clear();
			}
			else if (line.find("server_name") != std::string::npos)
				single_server_block.append(line + "\n");
			else if (line.find("server") == std::string::npos) //TODO may be simplified depending on invalidconfigexception change
				single_server_block.append(line + "\n");
		}
		// print_server_blocks();
	}

	void ConfigTokenizer::_tokenize_location_block(std::string line, std::istringstream &stream, std::string& single_server_block)
	{
		bool limit_on;

		single_server_block.append(line + "\n");
		while (std::getline(stream, line))
		{
			single_server_block.append(line + "\n");
			if(line.find("limit_except") != std::string::npos)
				limit_on = true;
			if (line.find("}") != std::string::npos)
			{
				if(limit_on == true)
					limit_on = false;
				else
					break;
			}
		}
	}

	void ConfigTokenizer::print_server_blocks(void)
	{

		for (size_t i = 0; i < _server_tokens.size(); i++)
		{
			std::cout << "\nEACH SERVER BLOCK" << std::endl;
			std::cout << _server_tokens[i] << std::endl;
		}
	}

	std::vector<std::string> ConfigTokenizer::get_server_tokens(void) const
	{
		return _server_tokens;
	}

} // namespace Config

#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

namespace Config
{

	class ConfigTokenizer
	{
	private:
		/* data */
		std::string _file_content;
		std::vector<std::string> _server_tokens;

		/* methods */
		void _tokenize_location_block(std::string line, std::istringstream &stream, std::string &single_server_block);

	public:
		ConfigTokenizer(std::string file_content);
		~ConfigTokenizer();

		void tokenize_server_blocks(void);
		void print_server_blocks(void);
		const std::vector<std::string>& get_server_tokens(void) const;
	};
} // namespace Config

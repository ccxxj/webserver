#ifndef _CONFIGPARSER_HPP_
# define _CONFIGPARSER_HPP_

# include <string>
# include <fstream>
# include <sstream>
# include <iostream>
# include <vector>
#include "ServerBlock.hpp"
#include "ConfigData.hpp"
//TODO should we have a ConfigValidator?
namespace Config {

	class ConfigParser {
	private:
		/* data */
		ConfigData *config_data;
		std::string file_path;
		std::string file_content;
		std::vector<std::string> server_tokens;

		/* methods */
		void open_and_read_file(void);
		void remove_comments(void);
		void tokenize_server_blocks(void);
		void print_server_blocks(void);
		ServerBlock &parse_server_block(std::string server_token);
		//ConfigParser(); do we need default constructor?

	public:
		ConfigParser(ConfigData *config_data, std::string file_path);
		~ConfigParser();

		void parse(void);

		class FailedToOpenException : public std::exception
		{
		public:
			const char *what() const throw()
			{
				return "Configuration file failed to open";
			}
		};
	};
}
#endif



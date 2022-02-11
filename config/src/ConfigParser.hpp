#ifndef ConfigParser_HPP
#define ConfigParser_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "ServerBlock.hpp"
#include "ConfigData.hpp"
#include "Utils.hpp"
//TODO should we have a ConfigValidator?
namespace Config
{

	class ConfigParser
	{
	private:
		/* data */
		enum Directives
		{
			LISTEN,
			SERVER_NAME,
			BODY_SIZE,
			ERROR_PAGE,
			RETURN,
			ROOT,
			LIMIT_EXCEPT,
			AUTOINDEX,
			ROUTE
		};
		ConfigData *config_data;
		std::string file_path;
		std::string file_content;
		std::vector<std::string> server_tokens;

		/* methods */
		void open_and_read_file(void);
		void remove_comments(void);
		void tokenize_server_blocks(void);
		void print_server_blocks(void);
		void parse_server_block(std::string server_token, ServerBlock &server);
		bool find_location(std::string line);
		int find_directive(std::string line);
		void parse_location_block(std::string line, std::istringstream &stream, ServerBlock &server);
		void parse_server_directive(std::string line, ServerBlock &server, int e_num);
		void parse_location_directive(std::string line, LocationBlock &location, int e_num);
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

		class InvalidConfigDirectiveException : public std::exception
		{
		public:
			const char *what() const throw()
			{
				return "Invalid directive in config";
			}
		};
	};
} // namespace Config
#endif

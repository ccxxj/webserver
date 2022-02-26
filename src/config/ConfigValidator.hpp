#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "ServerBlock.hpp"
#include "ConfigData.hpp"
#include "Utils.hpp"
#include "ConfigException.hpp"

namespace Config
{

	class ConfigValidator
	{
	private:
		/* data */
		std::string file_path;
		std::string file_content;

		/* methods */
		void open_and_read_file(void);
		void remove_comments(void);
		// void validate_config(void);
		// bool validate_server_line(std::string line);
		// void validate_location_block(std::string line, std::istringstream &stream);
		// bool validate_bracket_opening(std::string line, const std::string& keyword);
		// void check_outside_of_server_block(std::string line);
		//TODO ConfigValidator(); do we need default constructor?

	public:
		ConfigValidator(std::string file_path);
		~ConfigValidator();

		void validate(void);

		// class FailedToOpenException : public std::exception
		// {
		// public:
		// 	const char *what() const throw()
		// 	{
		// 		return "Configuration file failed to open";
		// 	}
		// };

		// class InvalidConfigDirectiveException : public std::exception
		// {
		// public:
		// 	const char *what() const throw()
		// 	{
		// 		return "Invalid directive in config";
		// 	}
		// };
	};
} // namespace Config

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stack>
#include "Utils.hpp"
#include "ConfigException.hpp"

namespace Config
{

	class ConfigValidator
	{
	private:
		/* data */
		std::string _file_path;
		std::string _file_content;

		/* methods */
		void _open_and_read_file(void);
		void _remove_comments_and_empty_lines(void);
		void _validate_server_blocks(void);
		bool _validate_server_opening(std::string line);
		void _check_outside_of_server_block(std::string line);
		void _validate_location_block(std::string line, std::istringstream &stream);
		void _are_brackets_balanced(void);
		void _check_semi_colon(std::string line);
		//TODO ConfigValidator(); do we need default constructor?

	public:
		ConfigValidator(std::string file_path);
		~ConfigValidator();

		void validate(void);
		std::string get_file_content(void) const;
	};
} // namespace Config

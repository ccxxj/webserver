#pragma once

#include <vector>
#include <string>
#include <sstream>

namespace Utility {

	std::vector<std::string> _split_line(const std::string& line, const char delimiter);
	std::string _trim(const std::string& s);
	bool contains_whitespace(std::string &str);
	void split_value(std::string line, std::vector<std::string> &vector_to_save);
	bool check_first_keyword(std::string line, std::string keyword);
	bool check_after_keyword(int last_pos, std::string str);
	void remove_first_keyword(std::string &line);
	void remove_white_space(std::string &line);
	std::vector<std::string> split_string_white_space(const std::string& str);
}

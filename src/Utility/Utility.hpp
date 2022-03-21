#pragma once

#include <vector>
#include <string>
#include <sstream>

namespace Utility {

	std::vector<std::string> _split_line(const std::string& line, const char delimiter);
	std::string _trim(const std::string& s);
	bool contains_whitespace(std::string &str);
	bool check_first_keyword(std::string line, std::string keyword);
	bool check_after_keyword(size_t last_pos, std::string str);
	void remove_white_space(std::string &line);
	void remove_last_of(char to_find, std::string &line);
	bool is_positive_integer(const std::string& s);
	std::string leading_trim(const std::string &s);
	std::string trailing_trim(const std::string &s);
	std::string trim_white_space(const std::string &s);
	std::vector<std::string> split_string_by_white_space(const std::string& str);
	bool is_hyphen(char c);
	const std::string to_string(const int code);
	std::string get_formatted_date();
}

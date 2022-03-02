#ifndef Utils_HPP
#define Utils_HPP
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

class Utils
{
public:
	static void split_value(std::string line, std::vector<std::string> &vector_to_save);
	static bool check_first_keyword(std::string line, std::string keyword);
	static bool check_after_keyword(int last_pos, std::string str);
	static void remove_first_keyword(std::string &line);
	static void remove_white_space(std::string &line);
	static std::vector<std::string> split_string_white_space(const std::string& str);
};
#endif

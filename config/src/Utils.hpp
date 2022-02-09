#ifndef Utils_HPP
#define Utils_HPP
#include <iostream>
#include <string>
#include <vector>

class Utils
{
public:
	static void split_value(std::string line, std::vector<std::string> &vector_to_save);
	static bool check_first_keyword(std::string line, std::string keyword);
	static void remove_first_keyword(std::string &line);
	static void remove_white_space(std::string &line);
};
#endif

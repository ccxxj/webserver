#pragma once

#include <vector>
#include <string>

namespace Utility {

	std::vector<std::string> _split_line(const std::string& line, const char delimiter);
	std::string _trim(const std::string& s);
	bool contains_whitespace(std::string &str);
}

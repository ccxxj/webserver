#include "Utils.hpp"

namespace HTTPRequest {


	std::vector<std::string> Utils::_split_line(const std::string& line, const char delimiter)
	{
		size_t start  = 0;
		std::vector<std::string> lines;

		while (true)
		{
			size_t match = line.find(delimiter, start);
			if (match == std::string::npos) {
				break;
			}
			size_t len = match - start;
			lines.push_back(line.substr(start, len));
			start = match + 1;
		}
		lines.push_back(line.substr(start, line.size()));
		return lines;
	}
}

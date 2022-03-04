#include "Utility.hpp"

namespace Utility {

	bool contains_whitespace(std::string& str) {
		size_t whitespace_found = str.find(' ', 0);
		size_t tab_found = str.find('\t', 0);
		return whitespace_found != std::string::npos || tab_found != std::string::npos;
	}

   std::vector<std::string> _split_line(const std::string& line, const char delimiter){
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
 
    std::string _trim(const std::string& s)
    {
        std::string::const_iterator start = s.begin();
        while (start != s.end() && std::isspace(*start)) {
            start++;
        }
        std::string::const_iterator end = s.end();
        do {
            end--;
        } while (std::distance(start, end) > 0 && std::isspace(*end));
    
        return std::string(start, end + 1);
    }
}

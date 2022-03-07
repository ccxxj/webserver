#include "Utility.hpp"
#include <algorithm>

namespace Utility
{

    bool contains_whitespace(std::string &str)
    {
        size_t whitespace_found = str.find(' ', 0);
        size_t tab_found = str.find('\t', 0);
        return whitespace_found != std::string::npos || tab_found != std::string::npos;
    }

    std::vector<std::string> _split_line(const std::string &line, const char delimiter)
    {
        size_t start = 0;
        std::vector<std::string> lines;

        while (true)
        {
            size_t match = line.find(delimiter, start);
            if (match == std::string::npos)
            {
                break;
            }
            size_t len = match - start;
            lines.push_back(line.substr(start, len));
            start = match + 1;
        }
        lines.push_back(line.substr(start, line.size()));
        return lines;
    }

    std::string _trim(const std::string &s)
    {
        std::string::const_iterator start = s.begin();
        while (start != s.end() && std::isspace(*start))
        {
            start++;
        }
        std::string::const_iterator end = s.end();
        do
        {
            end--;
        } while (std::distance(start, end) > 0 && std::isspace(*end));

        return std::string(start, end + 1);
    }

    void split_value(std::string line, std::vector<std::string> &vector_to_save)
    {
        int i = line.find_first_not_of("     ;{");
        int found = line.find_first_of("     ;{", i);
        int len = line.length();
        while (found < len && found != -1)
        {
            std::string sub = line.substr(i, found - i);
            vector_to_save.push_back(sub);
            if (line[found] == ';')
                break;
            else
            {
                i = line.find_first_not_of("     ;{", found + 1);
                found = line.find_first_of("     ;{", i);
            }
        }
    }

    bool check_first_keyword(std::string line, std::string keyword)
    {
        int first = line.find_first_not_of(" 	");
        if (first < 0)
            return false;
        int end = line.find_first_of(" 	;{", first + 1);
        if (line.substr(first, end - first).compare(keyword) == 0)
            return true;
        else
            return false;
    }

    void remove_first_keyword(std::string &line)
    {
        int first = line.find_first_not_of("    ");
        int end = line.find_first_of("  ", first + 1);
        line.erase(first, end - first);
    }

    void remove_white_space(std::string &temp)
    {
        temp.erase(std::remove_if(temp.begin(), temp.end(), ::isspace), temp.end());
    }

    void remove_last_of(char to_find, std::string &line)
    {
        size_t pos = line.find_last_of(to_find);
        line.erase(pos, pos + 1);
    }

    bool check_after_keyword(int last_pos, std::string str)
    {
        std::string tmp = str.substr(last_pos, str.length());
        remove_white_space(tmp);
        if (tmp.compare(";") != 0)
            return false;
        return true;
    }

    std::vector<std::string> split_string_white_space(const std::string &str)
    {
        std::vector<std::string> result;
        std::istringstream iss(str);

        for (std::string s; iss >> s;)
            result.push_back(s);
        return result;
    }

    bool is_positive_integer(const std::string& s)
    {
        std::string::const_iterator it = s.begin();
        while (it != s.end() && std::isdigit(*it)) 
            ++it;
        return !s.empty() && it == s.end();
    }   
} // namespace Utility

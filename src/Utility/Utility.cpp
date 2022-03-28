#include "Utility.hpp"
#include <algorithm>
#include <iostream>
#include <sys/time.h>
#include "../Constants.hpp"

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

    bool check_first_keyword(std::string line, std::string keyword)
    {
        size_t first = line.find_first_not_of(" \t");
        if (first < 0)
            return false;
        size_t end = line.find_first_of(" \t;{", first + 1);
        if (line.substr(first, end - first).compare(keyword) == 0)
            return true;
        else
            return false;
    }

    void remove_white_space(std::string &temp)
    {
        temp.erase(std::remove_if(temp.begin(), temp.end(), ::isspace), temp.end());
    }

    void remove_last_of(char to_find, std::string &line)
    {
        size_t pos = line.find_last_of(to_find);
        if (pos != std::string::npos)
            line.erase(pos, pos + 1);
    }

    bool check_after_keyword(size_t last_pos, std::string str)
    {
        std::string tmp = str.substr(last_pos, str.length());
        remove_white_space(tmp);
        if (tmp.compare(";") != 0)
            return false;
        return true;
    }

    std::vector<std::string> split_string_by_white_space(const std::string &str)
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

    std::string leading_trim(const std::string &s)
    {
        const std::string WHITESPACE = " \n\r\t\f\v";
        size_t start = s.find_first_not_of(WHITESPACE);
        return (start == std::string::npos) ? "" : s.substr(start);
    }

    std::string trailing_trim(const std::string &s)
    {
        const std::string WHITESPACE = " \n\r\t\f\v";
        size_t end = s.find_last_not_of(WHITESPACE);
        return (end == std::string::npos) ? "" : s.substr(0, end + 1);
    }

    std::string trim_white_space(const std::string &s)
    {
        const std::string WHITESPACE = " \n\r\t\f\v";
        return trailing_trim(leading_trim(s));
    }

    bool is_hyphen(char c) {
        return c == '-';
    }

	const std::string to_string(const int code) {
		std::string stringified_code;
		std::stringstream sstream;
		sstream << code;
		sstream >> stringified_code;
		return stringified_code;
	}

	std::string get_formatted_date() {
		struct timeval tv;
		char buf[32];
		struct tm *time;

		gettimeofday(&tv, NULL);
		time = gmtime(&tv.tv_sec);
		strftime(buf, 32, "%a, %d %b %Y %T GMT", time);
		std::string ret_val(buf);
		return ret_val;
	}

    std::string get_number_in_string(std::string& line) {
        if (line == "") {
            return "";
        }
        size_t non_number_position = line.find_first_not_of("0123456789ABCDEF", 0);
        if (non_number_position != std::string::npos) {
            return line.substr(0, non_number_position);
        }
        else {
            return line.substr(0, line.size());
        }
    }

    void logger(std::string str, std::string color)
	{
		struct tm *tm;
		time_t rawtime;
		char buf[32];

		time(&rawtime);
		tm = localtime(&rawtime);
		int ret = strftime(buf, 32, "%T", tm);
		buf[ret] = '\0';
		(void)color;
		// std::cout << GREEN << "[" << buf << "] " << RESET;
		// std::cout << color << str << RESET << std::endl;
		std::cout << "[" << buf << "] ";
		std::cout << str << std::endl;
	}
} // namespace Utility

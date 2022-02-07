#include "Utils.hpp"

void Utils::split_value(std::string str, std::vector<std::string> &a)
{
    int found = str.find_first_of(" 	;");
    int len = str.length();
    int i = 0;
    while(found < len && found != -1)
    {
        a.push_back(str.substr(i, found - i));
        i = str.find_first_not_of(" 	;", found + 1);
        found = str.find_first_of(" 	;", i);
        if (str[found] == ';')
        {
            a.push_back(str.substr(i, found - i));
            break;
        }
    }
}

bool Utils::check_first_keyword(std::string line, std::string keyword)
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

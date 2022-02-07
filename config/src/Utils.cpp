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


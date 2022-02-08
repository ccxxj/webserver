#include <iostream>
#include <string>
#include <vector>


class Utils
{
public: 
    static void split_value(std::string str, std::vector<std::string> &a);
    static bool check_first_keyword(std::string line, std::string keyword);
    static void remove_first_keyword(std::string &line);
    static void remove_white_space(std::string &line);
};

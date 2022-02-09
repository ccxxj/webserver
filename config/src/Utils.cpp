#include "Utils.hpp"

void Utils::split_value(std::string str, std::vector<std::string> &a)
{
	int found = str.find_first_of(" 	;");
	int len = str.length();
	int i = 0;
	a.push_back("fuckers");
	std::cout << "xx:  " << a[0] << std::endl;
	while (found < len && found != -1)
	{
		std::string sub = str.substr(i, found - i);
		a.push_back(sub);
		// a.push_back(str.substr(i, found - i));
		std::cout << "first sub in a: " << sub << std::endl;
		std::cout << "inside loop in a: " << a[1] << std::endl;

		i = str.find_first_not_of(" 	;", found + 1);
		found = str.find_first_of(" 	;", i);
		if (str[found] == ';')
		{
			std::string sub = str.substr(i, found - i);
			// std::string sub = "fu";
			std::cout << "sub: " << sub << std::endl;
			a.push_back(sub);
			std::cout << "inside loop in a: " << a[0] << std::endl;
			// a.push_back(str.substr(i, found - i));
			break;
		}
	}
	std::cout << "in a: " << a[0] << std::endl;
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

void Utils::remove_first_keyword(std::string &line)
{
	int first = line.find_first_not_of("    ");
	int end = line.find_first_of("  ", first + 1);
	line.erase(first, end - first);
}

void Utils::remove_white_space(std::string &temp)
{
	temp.erase(std::remove_if(temp.begin(), temp.end(), ::isspace), temp.end());
}

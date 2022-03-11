#include <iostream>
#include <map>
#include <string.h>
// #include <iostream>

int main()
{
	std::map<std::string, std::string> map;
	char *envp[3];
	map["AUTH_TYPE"];
	map["BAD"];
	std::string temp;
	std::map<std::string, std::string>::iterator it = map.begin();
	int i = 0;
	while(it != map.end())
	{
		// std::cout << it->first << "=" << it->second << "\n";
		// it++;
		temp = it->first + "=" + it->second + "\0";
		it++;
		envp[i] = strdup(temp.c_str());
		i++;
	}
	std::cout << envp[0] << "\n";
	std::cout << envp[1] << "\n";
}
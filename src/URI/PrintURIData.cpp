#include <iostream>
#include <map>

class URIData;

void print_URI_data(URIData uri)
{
	std::map<std::string, std::string>::iterator it = uri.get_authority();
	while(it != uri.get_authority().end())
	{
		std::cout << it->first << "::" << it->second << std::endl;
		it++;
	}
}
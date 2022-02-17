#include "URIData.hpp"

int main()
{
	std::string line;
	std::getline(std::cin, line);
	URIData uri(line);
	uri.set_authority();
	uri.print_URI_data();
}
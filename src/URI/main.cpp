#include "URIData.hpp"

int main()
{
	std::string line;
	// line = "aaaa/b=c/";
	std::getline(std::cin, line);
	URIData uri(line);
	uri.set_path();
	uri.print_URI_data();
}
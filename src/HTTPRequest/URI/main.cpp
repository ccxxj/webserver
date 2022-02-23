#include "URIParser.hpp"

//TODO add the compilation code to the main Makefile
// g++ main.cpp URIData.cpp Utils.cpp URIParser.cpp ../HTTP/Exceptions/RequestException.cpp -Wall -Wextra -Werror -std=c++98

int main()
{
	std::string line;
	std::getline(std::cin, line);
	URIData uri;
	URIParser parser(line);
	parser.parse(uri);
	uri.print_URI_data();
}
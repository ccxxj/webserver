#include "CGIRequest.hpp"

int main()
{
	CGIRequest cgi;
	std::cout << cgi.execute_cgi() << std::endl;
}
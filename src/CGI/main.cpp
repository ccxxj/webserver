#include "CGIRequest.hpp"

int main()
{
	std::cout << "check\n";
	CGIRequest cgi;
	cgi.execute_cgi();
}
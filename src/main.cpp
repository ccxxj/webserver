#include "Webserver.hpp"
#include <iostream>
#define DEFAULT_CONFIG "config/default-conf"

int main(int argc, char **argv) {
	std::string file_path;

	if (argc > 2)
	{
		std::cerr << "Invalid number of arguments" << std::endl;
		std::exit(EXIT_FAILURE);
	}
	if (argc == 1)
		file_path = DEFAULT_CONFIG;
	else if (argc == 2)
		file_path = argv[1];

	Webserver webserver(file_path);
	webserver.start();
	return 0;
}

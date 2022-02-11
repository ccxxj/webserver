#include "ConfigData.hpp"
#include "ConfigParser.hpp"
#include <iostream>
#define DEFAULT_CONFIG "./example_config_files/default-conf"

int main(int argc, char **argv)
{
	std::string file_path;

	if (argc > 2)
	{
		std::cout << "Invalid number of arguments" << std::endl;
		return (1);
	}
	if (argc == 1)
		file_path = DEFAULT_CONFIG;
	else if (argc == 2)
		file_path = argv[1];

	try
	{
		Config::ConfigData config;
		Config::ConfigParser parser(&config, file_path);
		parser.parse();
		config.print_servers_info();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}

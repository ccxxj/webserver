// #include "AConfigBlock.hpp"
// #include "LocationBlock.hpp"
#include "ConfigData.hpp"
#include "ConfigParser.hpp"
#include <iostream>
#define DEFAULT_CONFIG "./example_config_files/default-conf"

int main(int argc, char **argv)
{
	std::string	file_path;

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
		ConfigData		config;
		ConfigParser	parser(&config, file_path);
		parser.parse();
		std::cout << "end of parsee func" << std::endl;
		for (size_t i = 0; i < config.get_servers().size(); i++)
		{
			std::cout << "number: " << config.get_servers()[i].number << std::endl;
			// std::cout << "port: " << config.get_servers()[i].get_listen()[0] << std::endl;
			// std::cout << "route: " << config.get_servers()[i].get_location()[0].get_route() << std::endl;
		}
		std::cout << "end of main" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

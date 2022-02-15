#include "Webserver.hpp"

Webserver::Webserver(std::string file_path): _file_path(file_path)
{
}

Webserver::~Webserver()
{
}

void Webserver::start()
{
	// TODO: parse config
	//  if (config is ok) {
	//  HTTPServer server(&config);
	// }
	try
	{
		Config::ConfigData config;
		Config::ConfigParser parser(&config, _file_path);
		parser.parse();
		config.print_servers_info();
		HTTP::Server server(&config);
		server.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}

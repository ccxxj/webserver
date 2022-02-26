#include "Webserver.hpp"

Webserver::Webserver(std::string file_path): _file_path(file_path)
{
}

Webserver::~Webserver()
{
}

void Webserver::start()
{
	try
	{
		Config::ConfigData config;
		Config::ConfigValidator validator(_file_path);
		validator.validate();
		Config::ConfigTokenizer tokenizer(validator.get_file_content());
		tokenizer.tokenize_server_blocks();
		Config::ConfigParser parser(&config, tokenizer.get_server_tokens());
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

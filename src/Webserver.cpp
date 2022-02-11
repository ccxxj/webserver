#include "Webserver.hpp"
Webserver::Webserver(/* args */)
{
}

Webserver::~Webserver()
{
}

void Webserver::start() {
		//TODO: parse config
		// if (config is ok) {
		// HTTPServer server(&config);
	// }
		HTTP::Server server;
		server.run();
	}

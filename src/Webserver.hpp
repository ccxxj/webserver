#pragma once

#include <iostream>
#include <string>

#include "../src/HTTP/Server.hpp"
#include "../src/config/ConfigParser.hpp"

class Webserver
{
private:
	std::string _file_path;
public:
	Webserver(std::string file_path);
	~Webserver();
	void start();
};


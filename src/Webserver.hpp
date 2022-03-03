#pragma once

#include <iostream>
#include <string>

#include "../src/HTTP/Server.hpp"
#include "../src/config/ConfigParser.hpp"
#include "../src/config/ConfigValidator.hpp"
#include "../src/config/ConfigTokenizer.hpp"

class Webserver
{
private:
	std::string _file_path;
public:
	Webserver(std::string file_path);
	~Webserver();
	void start();
};


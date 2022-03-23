#include "Logger.hpp"

namespace Utility
{
	Logger::Logger(){};

	// TODO response log & request log & server num added to configparser and printed when matched in request handler
	void Logger::print(std::string& str, std::string& color)
	{
		struct tm *tm;
		time_t rawtime;
		char buf[32];

		time(&rawtime);
		tm = localtime(&rawtime);
		int ret = strftime(buf, 32, "%T", tm);
		buf[ret] = '\0';

		std::cout << PURPLE << "[" << buf << "] " << RESET;
		std::cout << color << str << RESET << std::endl;
	};
}

#pragma once

#include <iostream>
#include <string>
#include "../globals.hpp"

namespace Utility
{
	class Logger
	{
	public:
		Logger();
		void print(std::string& str, std::string& color);
	};

}

#pragma once

#include <exception>
#include <string>

namespace Config
{

	class ConfigException : public std::logic_error
	{
	public:
		ConfigException(std::string msg);
	};
} // namespace Config

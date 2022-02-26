#include "ConfigException.hpp"

namespace Config
{
	ConfigException::ConfigException(std::string msg) : std::logic_error(msg)
	{
	}
}

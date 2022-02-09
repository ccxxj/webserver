#ifndef ConfigData_HPP
# define ConfigData_HPP

#include "ServerBlock.hpp"
#include <vector>

// namespace Config {
	class ConfigData
	{
	private:
		/* data */
		std::vector<ServerBlock> _servers;
	public:
		ConfigData(/* args */);
		~ConfigData();

		std::vector<ServerBlock> &get_servers(void);
	};
// }
#endif

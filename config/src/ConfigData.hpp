#ifndef _CONFIGDATA_HPP_
# define _CONFIGDATA_HPP_

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
 




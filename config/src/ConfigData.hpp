#include "ServerBlock.hpp"
#include <vector>

class ConfigData
{
private:
	/* data */
	std::vector<ServerBlock> _servers;
public:
	ConfigData(/* args */);
	~ConfigData();
};



// #include "ServerBlock.hpp"
#include <vector>
class ServerBlock;
class ConfigData
{
private:
	/* data */
	std::vector<ServerBlock> _servers;
public:
	ConfigData(/* args */);
	~ConfigData();
};



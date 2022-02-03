// #include "AConfigBlock.hpp"
#include "LocationBlock.hpp"

class AConfigBlock;
class ServerBlock: public AConfigBlock
{
private:
	/* data */
	bool		_is_default;
	std::string	_listen;
	std::string	_server_name;
	std::vector<LocationBlock> _locations;
public:
	ServerBlock(/* args */);
	~ServerBlock();
};

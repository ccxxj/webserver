#pragma once
#include "AConfigBlock.hpp"
#include "LocationBlock.hpp"
# include <vector>
# include <string>
// class AConfigBlock;
// class LocationBlock;

class ServerBlock: public AConfigBlock
{
private:
	/* data */
	bool		_is_default; //TODO define the default server ??
	std::vector<std::string>	_listen;
	std::vector<std::string>	_server_name;
	std::vector<LocationBlock> _locations;
public:
	ServerBlock(/* args */);
	~ServerBlock();
	void set_listen(std::string str);
	void set_server_name(std::string str);
	std::vector<std::string> get_listen(std::string str) const;
	std::vector<std::string> get_server_name(std::string str) const;
};

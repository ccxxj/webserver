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
	std::vector<LocationBlock> _locations; //TODO get location
public:
	int	number;
	ServerBlock(/* args */);
	ServerBlock(const ServerBlock& other);
	~ServerBlock();
	ServerBlock &operator=(ServerBlock const & other);
	void set_listen(std::string str);
	void set_server_name(std::string str);
	std::vector<std::string> get_listen(void) const;
	std::vector<std::string> get_server_name(void) const;
};

#ifndef ServerBlock_HPP
#define ServerBlock_HPP
#include "AConfigBlock.hpp"
#include "LocationBlock.hpp"
#include <vector>
#include <string>

class ServerBlock: public AConfigBlock
{
private:
	/* data */
	bool _is_default; //TODO define the default server ??
	// int _client_max_body_size; //TODO is int okaay or do we need a bigger i.e. long?
	std::vector<std::string>	_listen;
	std::vector<std::string>	_server_name;
	std::vector<LocationBlock>	_locations; //TODO get location
public:
	int	number;
	ServerBlock(/* args */);
	ServerBlock(const ServerBlock& other);
	~ServerBlock();
	ServerBlock &operator=(const ServerBlock& other);

	//TODO client body size getter & setter
	void set_listen(std::string str);
	void set_server_name(std::string str);
	std::vector<std::string> get_listen(void) const;
	std::vector<std::string> get_server_name(void) const;
	std::vector<LocationBlock> &get_location(void); //TODO have it with const?
};
#endif

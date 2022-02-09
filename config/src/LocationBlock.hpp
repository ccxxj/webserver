#ifndef LocationBlock_HPP
#define LocationBlock_HPP
#include <vector>
#include <string>
#include "AConfigBlock.hpp"

class LocationBlock: public AConfigBlock
{
private:
	/* data */
	std::string	_route;
	std::vector<std::string>	_limit_except;
	int _autoindex;
public:
	LocationBlock(/* args */);
	~LocationBlock();
	void set_route(std::string str);
	void set_limit_except(std::string str);
	void set_autoindex(std::string str);
	std::string get_route(void) const;
	int get_autoindex(void) const;
	std::vector<std::string> get_limit_except(void) const;
};
#endif

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
	std::string get_route(std::string str) const;
	int get_autoindex(std::string str) const;
	std::vector<std::string> get_limit_except(void) const;
};


#include "LocationBlock.hpp"
#include "Utils.hpp"

LocationBlock::LocationBlock(/* args */)
{
}

LocationBlock::~LocationBlock()
{
}

void LocationBlock::set_route(std::string str)
{
	int first = str.find_first_not_of("     ");
    int last = str.find_first_of("  {", first);
    str = str.substr(first, last - first);
}

void LocationBlock::set_limit_except(std::string str)
{
    
}

std::string LocationBlock::get_route(std::string str) const
{
    return _route;
}

std::vector<std::string> LocationBlock::get_limit_except(void) const
{
    return _limit_except;
}


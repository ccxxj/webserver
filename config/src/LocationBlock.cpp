#include "LocationBlock.hpp"
#include "Utils.hpp"

#define NOT_SET -1
#define OFF 0
#define ON 1

LocationBlock::LocationBlock(/* args */)
{
    _autoindex = NOT_SET;//default the diretory listing is off
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

//TODO do we want to validate the Method keywords
void LocationBlock::set_limit_except(std::string str)
{
	Utils::split_value(str, _limit_except);
    int size = _limit_except.size();
    _limit_except.pop_back(); //pop out the "{" 
}

//TODO add exception handling on the key word other than on or off??
void LocationBlock::set_autoindex(std::string str)
{
    int first = str.find_first_not_of("     ");
    int last = str.find_first_of("  ;", first + 1);
    std::string keyword = str.substr(first, last - first);
    if(keyword.compare("on") == 0)
        _autoindex = ON;
    else if(keyword.compare("off") == 0)
        _autoindex = OFF;
    // else
    // {
        /* set as exception */
    // }
}

int LocationBlock::get_autoindex(std::string str) const
{
    return _autoindex;
}


std::string LocationBlock::get_route(std::string str) const
{
    return _route;
}

std::vector<std::string> LocationBlock::get_limit_except(void) const
{
    return _limit_except;
}


#include "LocationBlock.hpp"
#include "Utils.hpp"

#define NOT_SET -1
#define OFF 0
#define ON 1

LocationBlock::LocationBlock(/* args */)
{
    _autoindex = NOT_SET;//default the diretory listing is off
}

LocationBlock::LocationBlock(const LocationBlock& other)
{
    // std::cout << "LocationBlock copy constructor" << std::endl;
    *this = other;
}

const LocationBlock &	LocationBlock::operator=(LocationBlock const & other)
{
    _route = other._route;
    _limit_except = other._limit_except;
    _root = other._root;
    _return = other._return;
    _error_page = other._error_page;
	_autoindex = other._autoindex;
    // std::cout << "LocationBlock assign operator" << std::endl;
	return *this;
}

LocationBlock::~LocationBlock()
{
}

void LocationBlock::set_route(std::string str)
{
    Utils::remove_first_keyword(str);
	int first = str.find_first_not_of("     ");
    int last = str.find_first_of("  {", first);
    _route.assign(str.substr(first, last - first));
}

//TODO do we want to validate the Method keywords
void LocationBlock::set_limit_except(std::string str)
{
    Utils::remove_first_keyword(str);
	Utils::split_value(str, _limit_except);
    // int size = _limit_except.size();
    // (void)size; //TODO unused varibale do we need the int size?
    // _limit_except.pop_back(); //pop out the "{" 
}

//TODO add exception handling on the key word other than on or off??
void LocationBlock::set_autoindex(std::string str)
{
    Utils::remove_first_keyword(str);
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

int LocationBlock::get_autoindex() const
{
    return _autoindex;
}


std::string LocationBlock::get_route() const
{
    return _route;
}

std::vector<std::string> LocationBlock::get_limit_except(void) const
{
    return _limit_except;
}


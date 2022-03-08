#include "LocationBlock.hpp"
#include "../Utility/Utility.hpp"

#define NOT_SET -1
#define OFF 0
#define ON 1
namespace Config
{

    LocationBlock::LocationBlock()
    {
        _autoindex = NOT_SET; //default the diretory listing is off
        //TODO default client max body size check (nginx default 1M = 1000000 in decimal)
        _client_max_body_size = 1;
        _is_size_default = true;
    }

    LocationBlock::LocationBlock(const LocationBlock &other)
    {
        *this = other;
    }

    const LocationBlock &LocationBlock::operator=(LocationBlock const &other)
    {
        _route = other._route;
        _limit_except = other._limit_except;
        _autoindex = other._autoindex;
        _root = other._root;
        _return = other._return;
        _error_page = other._error_page;
        _client_max_body_size = other._client_max_body_size;
        _is_size_default = other._is_size_default;
        return *this;
    }

    LocationBlock::~LocationBlock()
    {
    }

    void LocationBlock::set_route(std::string str)
    {
        Utility::remove_first_keyword(str);
        int first = str.find_first_not_of("     ");
        int last = str.find_first_of("  {", first);
        _route.assign(str.substr(first, last - first));
    }

    //TODO do we want to validate the Method keywords
    void LocationBlock::set_limit_except(std::string str)
    {
        Utility::remove_first_keyword(str);
        Utility::split_value(str, _limit_except);
        // int size = _limit_except.size();
        // (void)size; //TODO unused varibale do we need the int size?
        // _limit_except.pop_back(); //pop out the "{"
    }

    //TODO add exception handling on the key word other than on or off??
    void LocationBlock::set_autoindex(std::string str)
    {
        Utility::remove_first_keyword(str);
        int first = str.find_first_not_of("     ");
        int last = str.find_first_of("  ;", first + 1);
        std::string keyword = str.substr(first, last - first);
        if (keyword.compare("on") == 0)
            _autoindex = ON;
        else if (keyword.compare("off") == 0)
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

    const std::string& LocationBlock::get_route() const
    {
        return _route;
    }

    const std::vector<std::string>& LocationBlock::get_limit_except(void) const
    {
        return _limit_except;
    }

} // namespace Config

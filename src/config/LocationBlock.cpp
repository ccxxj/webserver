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

    /* check methods */
    void LocationBlock::_check_limit_except(std::vector<std::string>& args) const
    {
        size_t size = args.size();
        if (size < 2)
            throw std::runtime_error("invalid number of arguments in limit_except directive");
        for (size_t i = 1; i < size; i++)
        {
            if (args[i] == "GET")
                continue ;
            if (args[i] == "POST")
                continue ;
            if (args[i] == "HEAD")
                continue ;
            if (args[i] == "DELETE")
                continue ;
            else
                throw std::runtime_error("invalid method " + args[i]);
        }   
    }

    /* getters & setters */
    void LocationBlock::set_route(std::string str)
    {
        Utility::remove_first_keyword(str);
        int first = str.find_first_not_of("     ");
        int last = str.find_first_of("  {", first);
        _route.assign(str.substr(first, last - first));
    }

    void LocationBlock::set_limit_except(std::string str)
    {
        Utility::remove_last_of('{', str);
        std::vector<std::string> args = Utility::split_string_white_space(str);
		_check_limit_except(args);
        for (size_t i = 1; i < args.size(); i++)
            _limit_except.push_back(args[i]);
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

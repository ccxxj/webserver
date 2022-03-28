#include "LocationBlock.hpp"
#include "../Utility/Utility.hpp"
#include "../Constants.hpp"


namespace Config
{

    LocationBlock::LocationBlock()
    {
        _autoindex = OFF; //default nginx
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

    size_t LocationBlock::_check_autoindex_syntax(std::vector<std::string>& args) const
    {
        if (args.size() != 2)
            throw std::runtime_error("invalid number of arguments in autoindex directive");
        else if (args[1].compare("on") == 0)
            return ON;
        else if (args[1].compare("off") == 0)
            return OFF;
        else
            throw std::runtime_error("invalid method " + args[1] + " in autoindex directive, it must be on or off");
    }

    /* getters & setters */
    void LocationBlock::set_route(std::string str)
    {
		Utility::remove_last_of('{', str);
        std::vector<std::string> args = Utility::split_string_by_white_space(str);
		if(args.size() == 2)
        	_route.assign(args[1]);
    }

    void LocationBlock::set_limit_except(std::string str)
    {
        Utility::remove_last_of('{', str);
        std::vector<std::string> args = Utility::split_string_by_white_space(str);
		_check_limit_except(args);
        for (size_t i = 1; i < args.size(); i++)
            _limit_except.push_back(args[i]);
    }

    void LocationBlock::set_autoindex(std::string str)
    {
        Utility::remove_last_of(';', str);
        std::vector<std::string> args = Utility::split_string_by_white_space(str);
        _autoindex = _check_autoindex_syntax(args);
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

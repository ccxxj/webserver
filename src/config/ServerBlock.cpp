#include "ServerBlock.hpp"
#include "../Utility/Utility.hpp"

#include <cstdlib>
namespace Config
{

    ServerBlock::ServerBlock()
    {
        _is_default = false;
        //TODO default client max body size check (nginx default 1M = 1000000 in decimal)
        _client_max_body_size = 1;
         _is_size_default = true;
    }

    ServerBlock::ServerBlock(const ServerBlock &other)
    {
        *this = other;
    }

    const ServerBlock &ServerBlock::operator=(ServerBlock const &other)
    {
        _is_default = other._is_default;
        _client_max_body_size = other._client_max_body_size;
        _listen = other._listen;
        _server_name = other._server_name;
        _locations = other._locations;
        _root = other._root;
        _return = other._return;
        _error_page = other._error_page;
        _is_size_default = other._is_size_default;
        return *this;
    }

    ServerBlock::~ServerBlock() {}

    std::string ServerBlock::_check_and_return_port(const std::string& str)
    {
        std::string tmp = str;
        Utility::remove_last_of(';', tmp);
        std::vector<std::string> listen_args = Utility::split_string_white_space(tmp);
        if (listen_args.size() != 2)
            throw std::runtime_error("invalid number of arguments in listen"); 
        _check_port_range(listen_args[1]);
        return listen_args[1];
    }

    void ServerBlock::_check_port_range(const std::string& port)
	{
        size_t port_num;
		std::string ipv6 = "[::]:";
		std::string str;

		size_t pos = port.find(ipv6);
		if (pos != std::string::npos)
			str = port.substr(pos + ipv6.length());
        else
            str = port;
        if(Utility::is_positive_integer(str) == false)
            throw std::runtime_error("host not found in directive listen " + port);
        port_num = std::atoi(str.c_str());
        if (port_num < 1 || port_num > 65535)
            throw std::runtime_error("host not found in directive listen " + port);
	}

    void ServerBlock::_check_duplicate_location_route(const std::string& route)
    {
        std::vector<LocationBlock> locations = get_location();
        for (size_t i = 0; i < locations.size(); i++)
            if (locations[i].get_route() == route)
                throw std::runtime_error("duplicate location " + route); 
    }

    void ServerBlock::set_listen(std::string str)
    {
        if(!_listen.insert(_check_and_return_port(str)).second)
            throw std::runtime_error("a duplicate " + str);
    }

    void ServerBlock::set_server_name(std::string str)
    {
        Utility::remove_first_keyword(str);
        Utility::split_value(str, _server_name);
    }

    void ServerBlock::set_default(bool value)
    {
        _is_default = value;
    }

    void ServerBlock::set_a_location(const LocationBlock &location)
    {
        _check_duplicate_location_route(location.get_route());
        _locations.push_back(location);
    }

    const std::set<std::string>& ServerBlock::get_listen() const
    {
        return _listen;
    }

    const std::vector<std::string>& ServerBlock::get_server_name() const
    {
        return _server_name;
    }

    const std::vector<LocationBlock> &ServerBlock::get_location(void) const
    {
        return _locations;
    }

    bool ServerBlock::get_default() const
    {
        return _is_default;
    }
} // namespace Config

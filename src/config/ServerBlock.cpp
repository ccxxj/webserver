#include "ServerBlock.hpp"
#include "../Utility/Utility.hpp"

#include <cstdlib>
namespace Config
{

    ServerBlock::ServerBlock()
    {
        _is_default = false;
        // std::cout << "ServerBlock construtor" << std::endl;
    }

    ServerBlock::ServerBlock(const ServerBlock &other)
    {
        // std::cout << "ServerBlock copy constructor" << std::endl;
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
        // std::cout << "ServerBlock assign operator" << std::endl;
        return *this;
    }

    ServerBlock::~ServerBlock()
    {
        //std::cout  << "ServerBlock destructor" << std::endl;
    }

    void ServerBlock::set_listen(std::string str)
    {
        Utility::remove_first_keyword(str);
        Utility::split_value(str, _listen);
    }

    void ServerBlock::set_server_name(std::string str)
    {
        Utility::remove_first_keyword(str);
        Utility::split_value(str, _server_name);
    }

    void ServerBlock::set_client_max_body_size(std::string str)
    {
        Utility::remove_first_keyword(str);
        int first = str.find_first_not_of("     ;");
        int last = str.find_first_of("     ;", first + 1);
        std::string temp = str.substr(first, last - first);
        _client_max_body_size = atoi(temp.c_str());
    }

    void ServerBlock::set_default(bool value)
    {
        _is_default = value;
    }

    void ServerBlock::set_a_location(const LocationBlock &location)
    {
        _locations.push_back(location);
    }

    int ServerBlock::get_client_max_body_size(void) const
    {
        return _client_max_body_size;
    }

    const std::vector<std::string>& ServerBlock::get_listen() const
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

#include "ServerBlock.hpp"
#include "Utils.hpp"

ServerBlock::ServerBlock(/* args */)
{
    (void)_is_default;
    std::cout << "created" << std::endl;
}

ServerBlock::ServerBlock(const ServerBlock& other)
{
    // std::copy(other._server_name.begin(), other._server_name.end(), this->_server_name.begin());
    // std::cout << get_server_name().size() << std::endl;
    // std::cout << get_server_name()[0] << " copy constructor" << std::endl;
    std::cout <<  "copy constructor" << std::endl;
    *this = other;
}

ServerBlock::~ServerBlock()
{
    // std::cout  << get_server_name()[0] << " destructed!" << std::endl;
    std::cout  << " destructed!" << std::endl;
}

void ServerBlock::set_listen(std::string str)
{
    Utils::split_value(str, _listen);
}
void ServerBlock::set_server_name(std::string str)
{
    Utils::split_value(str, _server_name);
}

std::vector<std::string> ServerBlock::get_listen() const
{
    return _listen;
}

std::vector<std::string> ServerBlock::get_server_name() const
{  
    return _server_name;
}


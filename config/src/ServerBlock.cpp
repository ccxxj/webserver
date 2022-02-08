#include "ServerBlock.hpp"
#include "Utils.hpp"

ServerBlock::ServerBlock(/* args */)
{
    (void)_is_default;
}

ServerBlock::~ServerBlock()
{
}

void ServerBlock::set_listen(std::string str)
{
    Utils::split_value(str, _listen);
}
void ServerBlock::set_server_name(std::string str)
{
    Utils::split_value(str, _server_name);
}

std::vector<std::string> ServerBlock::get_listen(std::string str) const
{
    return _listen;
}

std::vector<std::string> ServerBlock::get_server_name(std::string str) const
{  
    return _server_name;
}
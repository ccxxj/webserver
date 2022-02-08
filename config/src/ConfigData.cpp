#include "ConfigData.hpp"
// namespace Config {
    
ConfigData::ConfigData(/* args */)
{
}

ConfigData::~ConfigData()
{
}

std::vector<ServerBlock> &ConfigData::get_servers(void)
{
    // std::cout << "in get_servers" << std::endl;
    return (_servers);
}

// }


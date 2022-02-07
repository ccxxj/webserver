#include "ConfigData.hpp"
namespace Config {
    
ConfigData::ConfigData(/* args */)
{
}

ConfigData::~ConfigData()
{
}

std::vector<ServerBlock> &ConfigData::get_servers(void)
{
    return (_servers);
}

}
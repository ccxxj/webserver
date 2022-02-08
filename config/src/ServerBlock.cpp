#include "ServerBlock.hpp"
#include "Utils.hpp"

ServerBlock::ServerBlock(/* args */)
{
    (void)_is_default;
    number = rand();
    // std::cout << number << " created" << std::endl;
}

//TODO  assign operator and copy operator
ServerBlock::ServerBlock(const ServerBlock& other)
{
    // std::copy(other._server_name.begin(), other._server_name.end(), this->_server_name.begin());
    // std::cout << get_server_name().size() << std::endl;
    // std::cout << get_server_name()[0] << " copy constructor" << std::endl;
    // other.number = this->number;
    *this = other;
    // std::cout <<  number << " copy constructor" << std::endl;
    
}

ServerBlock &	ServerBlock::operator=(ServerBlock const & other)
{
	
	number = other.number;
    // std::cout << this->number << " assign operator" << std::endl;
	return *this;
}

ServerBlock::~ServerBlock()
{
    // std::cout  << get_server_name()[0] << " destructed!" << std::endl;
    // std::cout  << number << " destructed!" << std::endl;
}

void ServerBlock::set_listen(std::string str)
{
    std::cout << "in set listen line " << str << std::endl;
    Utils::remove_first_keyword(str);
    std::cout << "remmoved: " << str << std::endl;
    Utils::split_value(str, _listen);
    std::cout << "in set listen port: " << _listen[0] << std::endl;
}
void ServerBlock::set_server_name(std::string str)
{
    Utils::remove_first_keyword(str);
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

std::vector<LocationBlock> &ServerBlock::get_location(void)
{
    return _locations;
}


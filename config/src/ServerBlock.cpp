#include "ServerBlock.hpp"
#include "Utils.hpp"

ServerBlock::ServerBlock(/* args */)
{
    (void)_is_default;
    number = rand();
    // std::cout << number << " ServerBlock construtor" << std::endl;
}

ServerBlock::ServerBlock(const ServerBlock& other)
{
    // std::cout << other.number << " ServerBlock copy constructor" << std::endl;
    *this = other;
}

const ServerBlock &	ServerBlock::operator=(ServerBlock const & other)
{
	//TODO should I call the get functions?
	number = other.number;
    _listen = other._listen;
    _server_name = other._server_name;
    _locations = other._locations;
    _root = other._root;
    _return = other._return;
    _error_page = other._error_page;
	// std::cout << this->number << " ServerBlock assign operator" << std::endl;
	return *this;
}

ServerBlock::~ServerBlock()
{
    //std::cout  << number << " ServerBlock destructor" << std::endl;
}

void ServerBlock::set_listen(std::string str)
{
    Utils::remove_first_keyword(str);
    Utils::split_value(str, _listen);
}

void ServerBlock::set_server_name(std::string str)
{
    Utils::remove_first_keyword(str);
    Utils::split_value(str, _server_name);
}

void ServerBlock::set_client_max_body_size(std::string str)
{
    Utils::remove_first_keyword(str);
    int first = str.find_first_not_of("     ;");
    int last = str.find_first_of("     ;", first + 1);
    std::string temp = str.substr(first, last - first);
    _client_max_body_size = stoi(temp);
}

int ServerBlock::get_client_max_body_size(void) const
{
    return _client_max_body_size;
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


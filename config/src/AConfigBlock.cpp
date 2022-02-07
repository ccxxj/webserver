#include "AConfigBlock.hpp"
#include "Utils.hpp"

AConfigBlock::AConfigBlock(){}

AConfigBlock::~AConfigBlock(){}

/* getters & setters */
void AConfigBlock::set_return_value(std::string str)
{
	Utils::split_value(str, _return);	
}

void AConfigBlock::set_root_value(std::string str)
{
	Utils::split_value(str, _root);	
}

void AConfigBlock::set_error_page_value(std::string str)
{
	Utils::split_value(str, _error_page);	
}

std::vector<std::string> AConfigBlock::get_return(void) const
{
	return _return;
}

std::vector<std::string> AConfigBlock::get_root(void) const
{
	return _root;
}

std::vector<std::string> AConfigBlock::get_error_page(void) const
{
	return _error_page;
}

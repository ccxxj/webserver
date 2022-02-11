#include "AConfigBlock.hpp"
#include "Utils.hpp"

namespace Config
{

    AConfigBlock::AConfigBlock() {}

    AConfigBlock::AConfigBlock(const AConfigBlock &other)
    {
        // std::cout << "AConfigBlock copy constructor" << std::endl;
        *this = other;
    }

    const AConfigBlock &AConfigBlock::operator=(AConfigBlock const &other)
    {
        _return = other._return;
        _root = other._root;
        _error_page = other._error_page;
        // std::cout << "AConfigBlock assign operator" << std::endl;
        return *this;
    }

    AConfigBlock::~AConfigBlock() {}

    /* getters & setters */
    void AConfigBlock::set_return_value(std::string str)
    {
        Utils::remove_first_keyword(str);
        Utils::split_value(str, _return);
    }

    void AConfigBlock::set_root_value(std::string str)
    {
        Utils::remove_first_keyword(str);
        Utils::split_value(str, _root);
    }

    void AConfigBlock::set_error_page_value(std::string str)
    {
        Utils::remove_first_keyword(str);
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
} // namespace Config

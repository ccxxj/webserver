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
	void AConfigBlock::_check_return_syntax(std::string str)
	{
		std::vector<std::string> return_line;

		return_line = Utils::split_string_white_space(str);
		if (return_line.size() < 2 || return_line.size() < 3)
			throw std::runtime_error("invalid number of arguments in return directive");
	}
    /* getters & setters */
    void AConfigBlock::set_return_value(std::string str)
    {
		_check_return_syntax(str);
        Utils::remove_first_keyword(str);
        Utils::split_value(str, _return);
    }

    void AConfigBlock::set_root_value(std::string str)
    {
        if(!_root.empty())
            throw std::runtime_error("Invalid: root directive is duplicate");
        Utils::remove_first_keyword(str);
        int first = str.find_first_not_of("     ;");
        int last = str.find_first_of("     ;", first + 1);
        if(!Utils::check_after_keyword(last, str))
            throw std::runtime_error("Invalid: invalid number of arguments in root directive");
        _root = str.substr(first, last - first);
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

    std::string AConfigBlock::get_root(void) const
    {
        return _root;
    }

    std::vector<std::string> AConfigBlock::get_error_page(void) const
    {
        return _error_page;
    }
} // namespace Config

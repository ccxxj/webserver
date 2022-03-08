#include "AConfigBlock.hpp"
#include "../Utility/Utility.hpp"

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

	void AConfigBlock::_check_return_code(std::string code)
	{
		int code_num;

		if(Utility::is_positive_integer(code) == false)
            throw std::runtime_error("invalid return code " + code);
        code_num = std::atoi(code.c_str());
        if (code_num < 0 || code_num > 999)
            throw std::runtime_error("invalid return code " + code);
	}

	void AConfigBlock::_check_return_syntax(std::string str)
	{
		std::string tmp = str;
        Utility::remove_last_of(';', tmp);
		std::vector<std::string> return_line = Utility::split_string_white_space(tmp);
		//TODO nginx works with return + 1 arg. Reconsider this.
		if (return_line.size() < 2 || return_line.size() > 3)
			throw std::runtime_error("invalid number of arguments in return directive");
		_check_return_code(return_line[1]);
	}
    /* getters & setters */
    void AConfigBlock::set_return_value(std::string str)
    {
		_check_return_syntax(str);
        Utility::remove_first_keyword(str);
        Utility::split_value(str, _return);
    }

    void AConfigBlock::set_root_value(std::string str)
    {
        if(!_root.empty())
            throw std::runtime_error("Invalid: root directive is duplicate");
        Utility::remove_first_keyword(str);
        int first = str.find_first_not_of("     ;");
        int last = str.find_first_of("     ;", first + 1);
        if(!Utility::check_after_keyword(last, str))
            throw std::runtime_error("Invalid: invalid number of arguments in root directive");
        _root = str.substr(first, last - first);
    }

    void AConfigBlock::set_error_page_value(std::string str)
    {
        Utility::remove_first_keyword(str);
        Utility::split_value(str, _error_page);
    }

    const std::vector<std::string>& AConfigBlock::get_return(void) const
    {
        return _return;
    }

    const std::string& AConfigBlock::get_root(void) const
    {
        return _root;
    }

    const std::vector<std::string>& AConfigBlock::get_error_page(void) const
    {
        return _error_page;
    }
} // namespace Config

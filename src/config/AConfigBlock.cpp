#include "AConfigBlock.hpp"
#include "../Utility/Utility.hpp"

namespace Config
{

    AConfigBlock::AConfigBlock() {
        _is_size_default = false;
    }

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

    /* check methods */
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

    void AConfigBlock::_check_client_max_body_size_syntax(std::string& str)
    {
        std::string tmp = str;
        Utility::remove_last_of(';', tmp);
		std::vector<std::string> size_line = Utility::split_string_white_space(tmp);        
		if (size_line.size() != 1)
			throw std::runtime_error("invalid number of arguments in client_max_body_size");
		_check_size(size_line[0]);
    }

    void AConfigBlock::_check_size(std::string& size)
	{
		int size_num;

        //TODO assuming we will have M or m. K and G are now invalid.
        if(size[size.size() - 1] == 'M')
            Utility::remove_last_of('M', size); 
        else if(size[size.size() - 1] == 'm')
            Utility::remove_last_of('m', size);
		if(Utility::is_positive_integer(size) == false)
            throw std::runtime_error("client_max_body_size directive invalid value " + size);
        size_num = std::atoi(size.c_str());
        if (size_num < 0 || size_num > 8000) //TODO I got thins from MAX_SIZE_BODY. Discuss with teammates.
            throw std::runtime_error("client_max_body_size directive invalid valuee " + size);
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

    void AConfigBlock::set_client_max_body_size(std::string str)
    {
        if (_is_size_default == false)
            throw std::runtime_error("client_max_body_size directive is duplicate");
        Utility::remove_first_keyword(str);
        _check_client_max_body_size_syntax(str);
        int first = str.find_first_not_of("     ;");
        int last = str.find_first_of("     ;", first + 1);
        std::string temp = str.substr(first, last - first);
        _client_max_body_size = atoi(temp.c_str());
        _is_size_default = false;
    }

    int AConfigBlock::get_client_max_body_size(void) const
    {
        return _client_max_body_size;
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

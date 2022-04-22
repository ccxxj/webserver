#include "AConfigBlock.hpp"
#include "../Utility/Utility.hpp"
#include <cstdlib> // for atoi
#include "../Constants.hpp"

namespace Config
{

    AConfigBlock::AConfigBlock() {
        _is_size_default = false;
        _index_page = "index.html"; //default
    }

    AConfigBlock::AConfigBlock(const AConfigBlock &other)
    {
        *this = other;
    }

    const AConfigBlock &AConfigBlock::operator=(AConfigBlock const &other)
    {
        _return = other._return;
        _root = other._root;
        _error_page = other._error_page;
        _client_max_body_size = other._client_max_body_size;
        _is_size_default = other._is_size_default;
        _index_page = other._index_page;
        return *this;
    }

    AConfigBlock::~AConfigBlock() {}

    /* check methods */
	void AConfigBlock::_check_return_syntax(std::vector<std::string>& args) const
	{
		if (args.size() != 3)
			throw std::logic_error("invalid number of arguments in return directive");
		if(Utility::is_positive_integer(args[1]) == false)
            throw std::logic_error("invalid return code " + args[1]);
        size_t code_num = std::atoi(args[1].c_str());
        if (code_num < 0 || code_num > 999)
            throw std::out_of_range("invalid return code " + args[1]);
	}

	void AConfigBlock::_check_error_page_syntax(std::vector<std::string>& args) const
	{
		if (args.size() < 3)
			throw std::logic_error("invalid number of arguments in error_page directive");
		for (size_t i = 1; i < args.size() - 1; i++)
        {
            if(Utility::is_positive_integer(args[i]) == false)
                throw std::logic_error("invalid value " + args[i]);
            size_t code_num = std::atoi(args[i].c_str());
            if (code_num < 300 || code_num > 599)
                throw std::out_of_range("value " + args[i] + " must be between 300 and 599");
        }
	}

    void AConfigBlock::_check_root_syntax(std::vector<std::string>& args) const
	{
        if(!_root.empty())
            throw std::logic_error("root directive is duplicate");
		if (args.size() != 2)
		    throw std::logic_error("invalid number of arguments in root directive");
	}

    void AConfigBlock::_check_client_max_body_size_syntax(std::vector<std::string>& args)
    {
        if (_is_size_default == false)
            throw std::logic_error("client_max_body_size directive is duplicate");
		if (args.size() != 2)
			throw std::logic_error("invalid number of arguments in client_max_body_size");
		_check_size(args[1]);
    }

    void AConfigBlock::_check_size(std::string& size)
	{
        if(size[size.size() - 1] == 'K')
            Utility::remove_last_of('K', size);
        else if(size[size.size() - 1] == 'k')
            Utility::remove_last_of('k', size);
		if(Utility::is_positive_integer(size) == false)
            throw std::logic_error("client_max_body_size directive invalid value " + size);
        size_t size_num = std::atoi(size.c_str());
        if (size_num < 0 || size_num > Constants::DEFAULT_MAX_SIZE_BODY)
            throw std::out_of_range("client_max_body_size directive invalid valuee " + size);
	}

    /* setters */
    void AConfigBlock::set_return_value(std::string& str)
    {
        Utility::remove_last_of(';', str);
        std::vector<std::string> args = Utility::split_string_by_white_space(str);
		_check_return_syntax(args);
        for (size_t i = 1; i < args.size() - 1; i++)
            _return.insert(std::make_pair(std::atoi(args[i].c_str()), args[args.size() -1]));
    }

    void AConfigBlock::set_error_page_value(std::string& str)
    {
        Utility::remove_last_of(';', str);
        std::vector<std::string> args = Utility::split_string_by_white_space(str);
		_check_error_page_syntax(args);
        for (size_t i = 1; i < args.size() - 1; i++) {
            _error_page.insert(std::make_pair(std::atoi(args[i].c_str()), args[args.size() -1]));
        }
    }

    void AConfigBlock::set_root_value(std::string& str)
    {
        Utility::remove_last_of(';', str);
        std::vector<std::string> args = Utility::split_string_by_white_space(str);
		_check_root_syntax(args);
        _root = args[1];
    }

    void AConfigBlock::set_client_max_body_size(std::string& str)
    {

        Utility::remove_last_of(';', str);
        std::vector<std::string> args = Utility::split_string_by_white_space(str);
        _check_client_max_body_size_syntax(args);
        _client_max_body_size = atoi(args[1].c_str());
        _is_size_default = false;
    }

    void AConfigBlock::set_index_page(std::string& str)
    {
        Utility::remove_last_of(';', str);
        std::vector<std::string> args = Utility::split_string_by_white_space(str);
		if (args.size() != 2)
		    throw std::logic_error("invalid number of arguments in index directive");
        std::string extension = str.substr(str.find_last_of(".") + 1);
        if(extension != "html")
            throw std::logic_error("index page can only be an html");
        _index_page = args[1];
    }

    /* getters */
    int AConfigBlock::get_client_max_body_size(void) const
    {
        return _client_max_body_size;
    }

    bool AConfigBlock::get_is_size_default(void) const
    {
        return _is_size_default;
    }

    const std::map<int, std::string>& AConfigBlock::get_return(void) const
    {
        return _return;
    }

    const std::string& AConfigBlock::get_root(void) const
    {
        return _root;
    }

    const std::map<int, std::string>& AConfigBlock::get_error_page(void) const
    {
        return _error_page;
    }

    const std::string& AConfigBlock::get_index_page(void) const
    {
        return _index_page;
    }
} // namespace Config

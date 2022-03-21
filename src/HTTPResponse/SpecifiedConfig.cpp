#include "SpecifiedConfig.hpp"

namespace HTTPResponse
{

    SpecifiedConfig::SpecifiedConfig() {
		_has_specific_location = false;
    }

    SpecifiedConfig::SpecifiedConfig(const SpecifiedConfig &other)
    {
        *this = other;
    }

    const SpecifiedConfig &SpecifiedConfig::operator=(SpecifiedConfig const &other)
    {
        _return = other._return;
        _root = other._root;
        _error_page = other._error_page;
		_limit_except = other._limit_except;
		_route = other._route;
		_methods_line = other. _methods_line;
		_autoindex = other._autoindex;
        _client_max_body_size = other._client_max_body_size;
		_has_specific_location = other._has_specific_location;
        return *this;
    }

    SpecifiedConfig::~SpecifiedConfig() {}

    /* setters */
    void SpecifiedConfig::set_return_value(const std::vector<std::string>& returns)
    {
		for (std::vector<std::string>::const_iterator it = returns.begin(); it != returns.end(); it++)
			_return.push_back(*it);
    }

    void SpecifiedConfig::set_error_page_value(const std::vector<std::string>& errors)
    {
		for (std::vector<std::string>::const_iterator it = errors.begin(); it != errors.end(); it++)
			_error_page.push_back(*it);
    }

	void SpecifiedConfig::set_limit_except(const std::vector<std::string>& methods)
	{
		for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); it++)
			_limit_except.push_back(*it);
	}

    void SpecifiedConfig::set_root_value(const std::string& str)
    {
		_root = str;
    }

    void SpecifiedConfig::set_client_max_body_size(int client_max_body_size)
    {
        _client_max_body_size = client_max_body_size;
    }

	void SpecifiedConfig::set_route(const std::string& str)
    {
		_route = str;
    }

    void SpecifiedConfig::set_methods_line(std::vector<std::string> methods)
    {
		std::vector<std::string>::const_iterator it = methods.begin();
		while (it != methods.end()) {
			_methods_line += *it;
			it++;
			if (it != methods.end())
				_methods_line += ", ";
		}
    }

    void SpecifiedConfig::set_autoindex(int autoindex)
    {
		_autoindex = autoindex;
    }

	void SpecifiedConfig::set_specific_location(bool value)
	{
		_has_specific_location = value;
	}

    /* getters */
    int SpecifiedConfig::get_client_max_body_size(void) const
    {
        return _client_max_body_size;
    }

    const std::vector<std::string>& SpecifiedConfig::get_return(void) const
    {
        return _return;
    }

    const std::string& SpecifiedConfig::get_root(void) const
    {
        return _root;
    }

    const std::vector<std::string>& SpecifiedConfig::get_error_page(void) const
    {
        return _error_page;
    }

	int SpecifiedConfig::get_autoindex(void) const
    {
        return _autoindex;
    }

	const std::vector<std::string>& SpecifiedConfig::get_limit_except(void) const
    {
        return _limit_except;
    }

    const std::string& SpecifiedConfig::get_route(void) const
    {
        return _route;
    }

	const std::string& SpecifiedConfig::get_methods_line(void) const {
        return _methods_line;
    }

	bool SpecifiedConfig::has_specific_location(void) const {
        return _has_specific_location;
    }
} // namespace Config

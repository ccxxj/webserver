#include "SpecifiedConfig.hpp"

namespace HTTPResponse
{

    SpecifiedConfig::SpecifiedConfig() {
    }

    SpecifiedConfig::SpecifiedConfig(const SpecifiedConfig &other) {
        *this = other;
    }

    const SpecifiedConfig &SpecifiedConfig::operator=(SpecifiedConfig const &other) {
        _return = other._return;
        _root = other._root;
        _error_page = other._error_page;
		_limit_except = other._limit_except;
		_route = other._route;
		_methods_line = other. _methods_line;
		_autoindex = other._autoindex;
        _client_max_body_size = other._client_max_body_size;
        _index_page = other._index_page;
        return *this;
    }

    SpecifiedConfig::~SpecifiedConfig() {}

    /* setters */
    void SpecifiedConfig::set_return_value(const std::map<int, std::string>& returns) {
		for (std::map<int, std::string>::const_iterator it = returns.begin(); it != returns.end(); it++)
			_return.insert(*it);
    }

    void SpecifiedConfig::set_error_page_value(const std::map<int, std::string>& errors) {
		for (std::map<int, std::string>::const_iterator it = errors.begin(); it != errors.end(); it++)
			_error_page.insert(*it);
    }

	void SpecifiedConfig::set_limit_except(const std::vector<std::string>& methods) {
		for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); it++)
			_limit_except.push_back(*it);
	}

    void SpecifiedConfig::set_root_value(const std::string& str) {
		_root = str;
    }

    void SpecifiedConfig::set_client_max_body_size(int client_max_body_size) {
        _client_max_body_size = client_max_body_size;
    }

	void SpecifiedConfig::set_route(const std::string& str) {
		_route = str;
    }

    void SpecifiedConfig::set_methods_line(std::vector<std::string> methods) {
		std::vector<std::string>::const_iterator it = methods.begin();
		while (it != methods.end()) {
			_methods_line += *it;
			it++;
			if (it != methods.end())
				_methods_line += ", ";
		}
    }

    void SpecifiedConfig::set_upload_dir(const std::string& str) {
        _upload_dir = str;
    }

    void SpecifiedConfig::set_autoindex(int autoindex) {
		_autoindex = autoindex;
    }

    void SpecifiedConfig::set_id(int num) {
        _id = num;
    }

    void SpecifiedConfig::set_index_page(const std::string& str) {
        _index_page = str;
    }
    
    /* getters */
    int SpecifiedConfig::get_client_max_body_size(void) const {
        return _client_max_body_size;
    }

    const std::map<int, std::string>& SpecifiedConfig::get_return(void) const {
        return _return;
    }

    const std::string& SpecifiedConfig::get_root(void) const {
        return _root;
    }

    const std::map<int, std::string>& SpecifiedConfig::get_error_page(void) const {
        return _error_page;
    }

	int SpecifiedConfig::get_autoindex(void) const {
        return _autoindex;
    }

	const std::vector<std::string>& SpecifiedConfig::get_limit_except(void) const {
        return _limit_except;
    }

    const std::string& SpecifiedConfig::get_route(void) const {
        return _route;
    }

	const std::string& SpecifiedConfig::get_methods_line(void) const {
        return _methods_line;
    }

    const std::string& SpecifiedConfig::get_upload_dir() const {
        return _upload_dir;
    }

    int SpecifiedConfig::get_id(void) const {
        return _id;
    }

    const std::string& SpecifiedConfig::get_index_page(void) const {
        return _index_page;
    }
} // namespace Config

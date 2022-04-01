#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace HTTPResponse
{

	class SpecifiedConfig //contains all the information combined with ServerBlock & Location block values
	{
	private:
		std::string _root;
		std::map<int, std::string> _return;
		std::map<int, std::string> _error_page;
		std::vector<std::string> _limit_except;
		std::string _route;
		std::string _methods_line;
		int _autoindex;
		int _client_max_body_size;
		bool _has_specific_location;
		int _id;

	public:
		SpecifiedConfig();
		SpecifiedConfig(const SpecifiedConfig &other);
		const SpecifiedConfig &operator=(const SpecifiedConfig &other);
		virtual ~SpecifiedConfig();

		/* getters & setters */
		void set_return_value(const std::map<int, std::string>& returns);
		void set_root_value(const std::string& str);
		void set_route(const std::string& str);
		void set_error_page_value(const std::map<int, std::string>& errors);
		void set_client_max_body_size(int client_max_body_size);
		void set_limit_except(const std::vector<std::string>& methods);
		void set_methods_line(std::vector<std::string> methods);
		void set_autoindex(int autoindex);
		void set_specific_location(bool value);
		void set_id(int num);

		int get_client_max_body_size(void) const;
		bool get_is_size_default(void) const;
		const std::string& get_root(void) const;
		const std::map<int, std::string>& get_return(void) const;
		const std::map<int, std::string>& get_error_page(void) const;
		int get_autoindex(void) const;
		const std::vector<std::string>& get_limit_except(void) const;
		const std::string& get_route(void) const;
		const std::string& get_methods_line(void) const;
		bool has_specific_location(void) const;
		int get_id(void) const;
	};
} // namespace Config

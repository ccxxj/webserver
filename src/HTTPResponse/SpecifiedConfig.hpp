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
		std::string _index_page;
		std::string _route;
		std::string _methods_line;
		std::string _upload_dir;
		std::map<int, std::string> _return;
		std::map<int, std::string> _error_page;
		std::vector<std::string> _limit_except;
		std::vector<std::string> _cgi_extention_list;
		int _autoindex;
		int _client_max_body_size;
		int _id;

	public:
		SpecifiedConfig();
		SpecifiedConfig(const SpecifiedConfig &other);
		const SpecifiedConfig &operator=(const SpecifiedConfig &other);
		virtual ~SpecifiedConfig();

		/* getters & setters */
		
		void set_root_value(const std::string& str);
		void set_index_page(const std::string& str);
		void set_route(const std::string& str);
		void set_methods_line(std::vector<std::string> methods);
		void set_upload_dir(const std::string& str);
		void set_return_value(const std::map<int, std::string>& returns);
		void set_error_page_value(const std::map<int, std::string>& errors);
		void set_limit_except(const std::vector<std::string>& methods);
		void set_autoindex(int autoindex);
		void set_extention_list(const std::vector<std::string>& extentions);
		void set_client_max_body_size(int client_max_body_size);
		void set_id(int num);
	
		const std::string& get_root(void) const;
		const std::string& get_index_page(void) const;
		const std::string& get_route(void) const;
		const std::string& get_methods_line(void) const;
		const std::string& get_upload_dir(void) const;
		const std::map<int, std::string>& get_return(void) const;
		const std::map<int, std::string>& get_error_page(void) const;
		const std::vector<std::string>& get_limit_except(void) const;
		const std::vector<std::string>& get_extention_list(void) const;
		int get_autoindex(void) const;
		int get_client_max_body_size(void) const;
		int get_id(void) const;
		
	};
} // namespace HTTPResponse

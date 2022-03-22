#pragma once

#include <iostream>
#include <vector>

namespace Utility
{
	class File
	{
	private:
		std::string _path;
		std::string _dir;
		std::string _index_page;

	public:
		File(/* args */);
		~File();

		void set_path(const std::string &root, const std::vector<std::string> &uri_paths);
		void set_index_page(const std::string &str);
		const std::string& get_path(void);
		const std::string& get_index_page(void);
		std::string get_content(const std::string &str);
		bool exists(void);
		bool is_regular(void);
		bool is_directory(void);
		const std::string& list_directory(void);
		bool find_index_page();
		bool un_link(const std::string &str);
		std::string last_modified_info(const std::string &path);
		std::string last_modified_info();
	};

}

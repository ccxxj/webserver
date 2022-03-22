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

	public:
		File(/* args */);
		~File();

		void set_path(const std::string &root, const std::vector<std::string> &uri_paths);
		bool exists(void);
		bool is_directory(void);
		const std::string & list_directory(void);
	};

}

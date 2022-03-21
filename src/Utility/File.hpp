#pragma once

#include <string>
#include <vector>

namespace Utility
{
	class File
	{
	private:
		std::string _path;

	public:
		File(/* args */);
		~File();

		void set_path(const std::string &root, const std::vector<std::string> &uri_paths);
		bool is_directory(void);
	};

}

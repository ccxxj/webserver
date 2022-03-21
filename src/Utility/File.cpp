#include "File.hpp"
#include <sys/stat.h> //for stat that retrives information about a file
namespace Utility
{

	File::File(/* args */)
	{
	}

	File::~File()
	{
	}

	void File::set_path(const std::string &root, const std::vector<std::string> &uri_paths) {
		_path += root;
		for (std::vector<std::string>::const_iterator it = uri_paths.begin(); it != uri_paths.end(); it++)
			_path += (*it) + "/"; // FIXME what about the / in the end? check with directory and file
		//so the path is now "www/wordpress/index.html/"
	}

	bool File::is_directory(void) {
		struct stat s;
		if (stat(_path.c_str(), &s) == 0) // what happens if it doesn't exist? > should I check this first before everything else in serve_file
		{
			std::cout << s.st_mode << std::endl;
			return S_ISDIR(s.st_mode);
		}
		return false; //FIXME
	}

}

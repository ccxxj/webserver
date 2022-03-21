#include "File.hpp"
#include <sys/stat.h> //for stat that retrives information about a file 

//stat path check: relative to the current working directory of the calling process
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
		std::vector<std::string>::const_iterator it = uri_paths.begin(); 
		while (it != uri_paths.end()) {
			_path += *it;
			it++;
			if (it != uri_paths.end())
				_path += "/";
		}
		// FIXME what about the / in the end? check with directory and file
		//so the path is now "www/wordpress/index.html/"
	}

	bool File::exists(void) {
		struct stat buffer; 
		return stat(_path.c_str(), &buffer) == 0;
	}

	bool File::is_directory(void) {
		struct stat buffer; 
		stat(_path.c_str(), &buffer);
		return S_ISDIR(buffer.st_mode);
	}
}

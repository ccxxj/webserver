#include "File.hpp"
#include <sys/stat.h> //for stat that retrives information about a file
#include <dirent.h> // for dir functions

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

	const std::string & File::list_directory(void) {
		DIR *dir_p;
		struct dirent *entry;

		dir_p = opendir(_path.c_str());
		if (!dir_p) {
			std::cout << errno << std::endl;
			return NULL;
		}
		std::cout << "dir opened" << std::endl;
		while ((entry = readdir(dir_p))) {
			std::cout << entry->d_name << std::endl;
			_dir += "<p>" + //entry->d_name //toString() + "<\p>"
		}
		closedir(dir_p);
		return NULL;

	}
}

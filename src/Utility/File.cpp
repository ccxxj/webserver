#include "File.hpp"
#include <sys/stat.h> //for stat that retrives information about a file
#include <dirent.h> // for dir functions
#include <fcntl.h> // open files
#include <unistd.h>

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
		return  stat(_path.c_str(), &buffer) == 0;
	}

	bool File::is_directory(void) {
		struct stat buffer;
		stat(_path.c_str(), &buffer);
		return S_ISDIR(buffer.st_mode);
	}

	const std::string & File::list_directory(void) { //TODO if you want more add last modified and size option to html
		DIR *dir_p;
		struct dirent *entry;
		std::string	tmp;

		dir_p = opendir(_path.c_str());
		if (!dir_p) 
			return _dir;
		_dir += "<html>\r\n<h2>" + _path + "</h2><ul>";
		while ((entry = readdir(dir_p))) {
			// tmp = _path + "/" + entry->d_name;
			// _dir += "<li><a href=\"";  
			// _dir += tmp + "\">"; //TODO do we need to add links to files?
			_dir += "<li><a>";  
			if (entry->d_type == DT_DIR)
				_dir += "Dir  : ";
			else
				_dir += "File : ";
			_dir += entry->d_name;
			_dir +=  "</a></li>";;
		}
		_dir += "</ul>";
		closedir(dir_p);
		return _dir;
	}

	bool File::find_index_page() {
		DIR *dir_p;
		struct dirent *entry;
		std::string	index = "index.html";

		dir_p = opendir(_path.c_str());
		if (!dir_p) 
			return false;
		while ((entry = readdir(dir_p))) {
			if (entry->d_name == index) {
				_index_page = std::string(entry->d_name);
				closedir(dir_p);
				return true;
			}
		}
		return false;
	}

	std::string File::get_content(const std::string &str) {
		std::string file_content;
		char buf[4096 + 1];
  		int ret;

		int fd = open(str.c_str(), O_RDONLY);
		if (fd == -1)
			return "";
		while ((ret = read(fd, buf, 4096)) != 0) {
			if (ret == -1)
				return "";
			buf[ret] = '\0';
			file_content.insert(file_content.length(), buf, ret);
		}
		return file_content;
	}

	std::string File::last_modified_info() {
		struct stat statbuf;
		struct tm	*time;
		char buf[32];

		stat(_path.c_str(), &statbuf);
		time = gmtime(&statbuf.st_mtime);
		strftime(buf, 32, "%a, %d %b %Y %T GMT", time);
		std::string ret_val(buf);
		return ret_val;
	}

	std::string File::last_modified_info(const std::string &path) {
		struct stat statbuf;
		struct tm	*time;
		char buf[32];

		stat(path.c_str(), &statbuf);
		time = gmtime(&statbuf.st_mtime);
		strftime(buf, 32, "%a, %d %b %Y %T GMT", time);
		std::string ret_val(buf);
		return ret_val;
	}

	void File::set_index_page(const std::string &str) {
		_index_page = str;
	}

	const std::string & File::get_index_page(void) {
		return _index_page;
	}

	const std::string & File::get_path(void) {
		return _path;
	}
}

#include "File.hpp"
#include "../Utility/Utility.hpp"
#include "../Constants.hpp"

#include <sys/stat.h> //for stat that retrives information about a file
#include <dirent.h> // for dir functions
#include <fcntl.h> // open files
#include <unistd.h>
#include <errno.h>
#include <string.h> //for strerror

//stat path check: relative to the current working directory of the calling process
namespace Utility
{
	MimeTypes File::_mimes;

	File::File(/* args */)
	{
	}

	File::~File()
	{
	}

	void File::set_path(const std::string &root, const std::vector<std::string> &uri_paths) {
		_root = root;
		_path += root;
		set_target(uri_paths);
		_path += _target;
		//so if root is "www" and the target is "wordpress/index.html" the path is now "www/wordpress/index.html"
	}

	void File::set_target(const std::vector<std::string> &uri_paths) {
		std::vector<std::string>::const_iterator it = uri_paths.begin();
		if (uri_paths.size() == 1 && (*it).empty()) {
			_target += "/";
			return ;
		}
		while (it != uri_paths.end()) {
			_target += *it;
			it++;
			if (it != uri_paths.end())
				_target += "/";
		}
	}

	void File::set_root(const std::string &root) {
		_root = root;
	}

	bool File::exists(void) {
		struct stat buffer;
		return  stat(_path.c_str(), &buffer) == 0;
	}

	bool File::is_regular(void) {
		struct stat buffer;
		stat(_path.c_str(), &buffer);
		return S_ISREG(buffer.st_mode);
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
			Utility::logger("DEBUG opendir : " + std::string(strerror(errno)), RED);
			return _dir;
		}
		_dir += "<html>\r\n<h2> Index of " + _target + "</h2>";
		while ((entry = readdir(dir_p))) {
			_dir += "<li><a href=\"";
			_dir += _target + "/" + entry->d_name + "\">";
			if (entry->d_type == DT_DIR)
				_dir += "Dir  : ";
			else
				_dir += "File : ";
			_dir += entry->d_name;
			_dir +=  "</a></li>";;
		}
		closedir(dir_p);
		return _dir;
	}

	bool File::find_index_page(const std::string& index) {
		DIR *dir_p;
		struct dirent *entry;

		dir_p = opendir(_path.c_str());
		if (!dir_p) {
			Utility::logger("DEBUG mkdir : " + std::string(strerror(errno)), RED);
			return false;
		}
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
		if (fd == -1) {
			Utility::logger("DEBUG open : " + std::string(strerror(errno)) , RED);
			return "Forbidden";
		}
		while ((ret = read(fd, buf, 4096)) != 0) {
			if (ret == -1) {
				Utility::logger("DEBUG read : " + std::string(strerror(errno)), RED);
				return "Forbidden";
			}
			buf[ret] = '\0';
			file_content.insert(file_content.length(), buf, ret);
		}
		return file_content;
	}

	bool File::un_link(const std::string &str) {
		if (unlink(str.c_str()) == -1) {
			Utility::logger("DEBUG unlink : " + std::string(strerror(errno)), RED);
			return false;
		}
		return true;
	}

	bool File::create_dir() {
		if (mkdir(_path.c_str(), 0755) == -1) {
			Utility::logger("DEBUG mkdir : " + std::string(strerror(errno)), RED);
			return false;
		}
		return true;
	}

	bool File::create_random_named_file_put_msg_body_in(const std::string& str) {
		//just to create get the fiel count in dir to create a random name
		struct dirent *entry;
		int i = 0;
		DIR *dir_p = opendir(_path.c_str());
		if (!dir_p) {
			Utility::logger("DEBUG opendir : " + std::string(strerror(errno)), RED);
			return false;
		}
		while ((entry = readdir(dir_p))) {
			i++;
		}
		closedir(dir_p);

		//create the file
		int fd = open((_path + "/file" + Utility::to_string(i - 1)).c_str(), O_CREAT | O_RDWR | O_TRUNC, 00755);
		if (str.length() &&  write(fd, str.c_str(), str.length()) <= 0) // write the request body to the file
			return false;
		return true;
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

		int ret = stat(path.c_str(), &statbuf);
		if (ret != 0)
			return "";
		time = gmtime(&statbuf.st_mtime);
		strftime(buf, 32, "%a, %d %b %Y %T GMT", time);
		std::string ret_val(buf); 
		return ret_val;
	}

	std::string File::get_mime_type(const std::string& str) {
		return _mimes.get_mime_type(str);
	}

	std::string File::extract_file_name(const std::string &str) {
		//str example: form-data; name=\"new_file\"; filename=\"specific_name_for.pdf\";
		std::string key = "filename=\"";
		size_t match = str.find(key);
		std::string file_name = str.substr(match + key.size(), str.size());
		size_t last_quote = file_name.find_last_not_of("\"");
		file_name  = file_name.substr(0, last_quote + 1);
		return file_name;
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

	const std::string & File::get_root(void) {
		return _root;
	}

	const std::string & File::get_target(void) {
		return _target;
	}
}

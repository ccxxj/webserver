#include "ConfigValidator.hpp"
#include <stdlib.h>
namespace Config
{

	ConfigValidator::ConfigValidator(std::string file_path) :file_path(file_path)
	{
	}

	ConfigValidator::~ConfigValidator()
	{
	}

	void ConfigValidator::open_and_read_file(void)
	{
		std::ifstream file_stream;
		std::string line;

		file_stream.open(file_path.c_str());
		if (!file_stream.is_open())
			throw ConfigException("Configuration file failed to open");
		while (std::getline(file_stream, line))
			file_content.append(line + "\n");
		file_stream.close();
	}

	void ConfigValidator::remove_comments(void)
	{
		std::istringstream stream(file_content);
		std::string line;
		size_t hashtag_pos;

		file_content.clear();
		while (std::getline(stream, line))
		{
			hashtag_pos = line.find('#');
			if (hashtag_pos != std::string::npos)
				line.erase(hashtag_pos, std::string::npos);
			if (!line.empty())
				file_content.append(line + "\n");
		}
	}
	
	//TODO it still parses without ; at the end of lines!
	void ConfigValidator::validate(void)
	{
		open_and_read_file();
		remove_comments();
		// validate_config();
	}

} // namespace Config

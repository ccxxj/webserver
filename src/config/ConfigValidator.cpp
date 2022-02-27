#include "ConfigValidator.hpp"
#include <stdlib.h>
namespace Config
{

	ConfigValidator::ConfigValidator(std::string file_path) :_file_path(file_path)
	{
	}

	ConfigValidator::~ConfigValidator()
	{
	}

	void ConfigValidator::_open_and_read_file(void)
	{
		std::ifstream file_stream;
		std::string line;

		file_stream.open(_file_path.c_str());
		if (!file_stream.is_open())
			throw ConfigException("Configuration file failed to open");
		while (std::getline(file_stream, line))
			_file_content.append(line + "\n");
		file_stream.close();
	}

	void ConfigValidator::_remove_comments_and_empty_lines(void)
	{
		std::istringstream stream(_file_content);
		std::string line;
		size_t hashtag_pos;
		std::string tmp;

		_file_content.clear();
		while (std::getline(stream, line))
		{
			hashtag_pos = line.find('#');
			if (hashtag_pos != std::string::npos)
				line.erase(hashtag_pos, std::string::npos);
			tmp = line;
			Utils::remove_white_space(tmp);
			if (!line.empty() && !tmp.empty())
				_file_content.append(line + "\n");
		}
	}

	void ConfigValidator::_are_brackets_balanced(void)
	{
		std::stack<char> brackets;

		for (size_t i = 0; i < _file_content.length(); i++)
		{
			if(_file_content[i] == '{')
			{
				brackets.push('{');
				continue;
			}
			if(brackets.empty() && _file_content[i] == '}')
				throw ConfigException("Invalid-Config: Unbalanced brackets");
			if(_file_content[i] == '}')
				brackets.pop();
		}
		if(!brackets.empty())
			throw ConfigException("Invalid-Config: Unbalanced brackets");
	}

	void ConfigValidator::_check_semi_colon(std::string line)
	{
		size_t length;
		std::string tmp;

		tmp = line;
		Utils::remove_white_space(tmp);
		length = tmp.length();
		if (tmp[length - 1] != ';')
			throw ConfigException("Invalid-Config: Missing semicolon");
	}

	bool ConfigValidator::_validate_server_opening(std::string line)
	{
		std::string temp = line;
		Utils::remove_white_space(temp);
		if(temp.compare("server{") != 0)
			throw ConfigException("Server opening line is invalid in config");
		return true;
	}

	void ConfigValidator::_check_outside_of_server_block(std::string line)
	{
		std::string temp = line;
		Utils::remove_white_space(temp);
		if(!temp.empty())
			throw ConfigException("Invalid-Config: Information outside of server blocks");
	}

	void ConfigValidator::_validate_location_block(std::string line, std::istringstream &stream)
	{
		bool limit_on;

		//TODO validate location block opening
		limit_on = false;
		while (std::getline(stream, line))
		{
			if (line.find("limit_except") != std::string::npos)
				limit_on = true;
			else if (line.find("}") != std::string::npos)
			{
				if (limit_on == true)
					limit_on = false;
				else
					break;
			}
			else
				_check_semi_colon(line);
		}
	}

	void ConfigValidator::_validate_server_blocks(void)
	{
		bool server_on;
		std::string line;
		std::istringstream stream(_file_content);

		server_on = false;
		while (std::getline(stream, line))
		{
			if (line.find("server") != std::string::npos && line.find("server_name") == std::string::npos)
				server_on = _validate_server_opening(line);
			else if(server_on == false)
				_check_outside_of_server_block(line);
			else if (line.find("location") != std::string::npos)
				_validate_location_block(line, stream);
			else if (line.find("}") != std::string::npos && server_on == true)
				server_on = false;
			else
				_check_semi_colon(line);
		}
	}
	//TODO it still parses without ; at the end of lines!
	void ConfigValidator::validate(void)
	{
		_open_and_read_file();
		_remove_comments_and_empty_lines();
		_are_brackets_balanced();
		_validate_server_blocks();
		//TODO no listening ports?
		//TODO validate } lines.
	}

	std::string ConfigValidator::get_file_content() const
	{
		return _file_content;
	}

} // namespace Config


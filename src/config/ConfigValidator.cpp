#include "ConfigValidator.hpp"
#include <stdlib.h>
#include "../Utility/Utility.hpp"

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
			throw std::runtime_error("Configuration file failed to open");
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
			Utility::remove_white_space(tmp);
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
				throw std::runtime_error("Invalid-Config: Unbalanced brackets");
			if(_file_content[i] == '}')
				brackets.pop();
		}
		if(!brackets.empty())
			throw std::runtime_error("Invalid-Config: Unbalanced brackets");
	}

	void ConfigValidator::_check_semi_colon(std::string line)
	{
		size_t length;
		std::string tmp;

		tmp = line;
		Utility::remove_white_space(tmp);
		length = tmp.length();
		if (tmp[length - 1] != ';')
			throw std::runtime_error("Invalid-Config: Missing semicolon");
	}

	void ConfigValidator::_check_closing_bracket_line(std::string line)
	{
		std::string temp = line;
		Utility::remove_white_space(temp);
		if (temp.compare("}") != 0)
			throw std::runtime_error("Invalid-Config: Closing bracket");
	}

	bool ConfigValidator::_validate_server_opening(std::string line)
	{
		std::string temp = line;
		Utility::remove_white_space(temp);
		if (temp.compare("server{") != 0)
			throw std::runtime_error("Server opening line is invalid in config");
		return true;
	}

	void ConfigValidator::_check_outside_of_server_block(std::string line)
	{
		std::string temp = line;
		Utility::remove_white_space(temp);
		if(!temp.empty())
			throw std::runtime_error("Invalid-Config: Information outside of server blocks");
	}

	void ConfigValidator::_validate_location_opening(std::string line)
	{
		std::vector<std::string> location_split;
		size_t size;

		location_split = Utility::split_string_white_space(line);
		size = location_split.size();
		if (size == 3)
		{
			if (location_split[0].compare("location") != 0)
				throw std::runtime_error("Invalid-Config: Location opening");
			if (location_split[2].compare("{") != 0)
				throw std::runtime_error("Invalid-Config: Location opening");
		}
		else if (size == 2)
		{
			if (location_split[0].compare("location") != 0)
				throw std::runtime_error("Invalid-Config: Location opening");
			if (location_split[1].find("{") != location_split[1].length() - 1)
				throw std::runtime_error("Invalid-Config: Location opening");
			if (location_split[1].compare("{") == 0)
				throw std::runtime_error("Invalid-Config: Location opening");
		}
		else
			throw std::runtime_error("Invalid-Config: Location opening");
	}

	void ConfigValidator::_validate_deny_directive(std::string line)
	{
		_check_semi_colon(line);
		Utility::remove_last_of(';', line);
		std::vector<std::string> deny_split = Utility::split_string_white_space(line);
		if (deny_split.size() != 2)
			throw std::runtime_error("invalid number of arguments in deny directive");	
		if (deny_split[0].compare("deny") != 0)
			throw std::runtime_error("unknown directive " + deny_split[0]);
		if (deny_split[1].compare("all") != 0)
			throw std::runtime_error("invalid parameter " + deny_split[1]);
	}

	//TODO we decided to have simple limit except limit_except GET { deny all; }. So, no allow line?
	void ConfigValidator::_validate_limit_except(std::string line, std::istringstream &stream)
	{
		std::vector<std::string> limit_except_split = Utility::split_string_white_space(line);
		if (limit_except_split[0].compare("limit_except") != 0)
			throw std::runtime_error("unknown directive " + limit_except_split[0]);
		while (std::getline(stream, line))
		{
			if (line.find("deny") != std::string::npos)
				_validate_deny_directive(line);
			else if (line.find("}") != std::string::npos)
			{
				_check_closing_bracket_line(line);
				break;
			}
			else
				throw std::runtime_error("unknown directive " + line);	
		}
	}

	void ConfigValidator::_validate_location_block(std::string line, std::istringstream &stream)
	{
		_validate_location_opening(line);
		while (std::getline(stream, line))
		{
			if (line.find("limit_except") != std::string::npos)
				_validate_limit_except(line, stream);
			else if (line.find("}") != std::string::npos)
			{
				_check_closing_bracket_line(line);
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
			{
				_check_closing_bracket_line(line);
				server_on = false;
			}
			else if (line.find("limit_except") != std::string::npos && server_on == true)
				throw std::runtime_error("limit_except directive is not allowed here");
			else
				_check_semi_colon(line);
		}
	}

	void ConfigValidator::validate(void)
	{
		_open_and_read_file();
		_remove_comments_and_empty_lines();
		_are_brackets_balanced();
		_validate_server_blocks();
	}

	const std::string& ConfigValidator::get_file_content() const
	{
		return _file_content;
	}

} // namespace Config


#include "URIParser.hpp"

URIData::URIData(std::string input_URI)
{
	this->input_URI = input_URI;
}

URIData::~URIData(){}


void URIData::set_authority(void)
{
	std::string authority;
	int len = input_URI.length();
	int delimiter_slash_position = input_URI.find_first_of("/");
	if(delimiter_slash_position == -1)
	{
		delimiter_slash_position = len;
		authority = input_URI.substr(0, delimiter_slash_position);
	}
	else
		authority = input_URI.substr(0, delimiter_slash_position);
	int delimiter_at_position = authority.find_first_of("@");
	if(delimiter_at_position < delimiter_slash_position && delimiter_at_position != -1)
	{
		_authority["userinfo"] = authority.substr(0, delimiter_at_position);
		authority.erase(0, delimiter_at_position + 1);
	}
	int delimiter_comma_position = authority.find_first_of(":");
	if(delimiter_comma_position < delimiter_slash_position && delimiter_comma_position != -1)
	{
		int delimiter_slash_position = authority.find_first_of("/");
		if(delimiter_slash_position == -1)
		{
			int port_len = authority.length() - delimiter_comma_position - 1;
			_authority["port"] = authority.substr(delimiter_comma_position + 1, port_len);
			authority.erase(delimiter_comma_position, port_len + 1);
		}
		else{
			int port_len = delimiter_slash_position - delimiter_comma_position - 1;
			_authority["port"] = authority.substr(delimiter_comma_position + 1, port_len);
			authority.erase(delimiter_comma_position, port_len + 2);
		}

	}
	_authority["host"] = authority.substr(0, delimiter_slash_position);
	input_URI.erase(0, delimiter_slash_position + 1);
	std::cout << input_URI << std::endl;
}

const std::map<std::string, std::string> URIData::get_authority(void) const
{
	return _authority;
}

const void URIData::print_URI_data(void)
{
	std::map<std::string, std::string>::iterator it = _authority.begin();
	while(it != _authority.end())
	{
		std::cout << it->first << "::" << it->second << std::endl;
		it++;
	}
}



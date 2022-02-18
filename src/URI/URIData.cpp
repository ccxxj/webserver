#include "URIParser.hpp"

URIData::URIData(std::string input_URI)
{
	this->input_URI = input_URI;
}

URIData::~URIData(){}

void URIData::set_path(void)
{
	int len = input_URI.length();
	int delimiter_query_position = input_URI.find_first_of("?");
	if(delimiter_query_position != -1 && delimiter_query_position != len - 1)
	{
		_query.push_back(input_URI.substr(delimiter_query_position + 1, len - delimiter_query_position - 1));
		input_URI.erase(delimiter_query_position, len - delimiter_query_position);
	}
	while(input_URI.length() > 0)
	{
		int delimiter_slash_position = input_URI.find_first_of("/");
		if(delimiter_slash_position == -1)
		{
			len = input_URI.length();
			_path.push_back(input_URI.substr(0, len));
			input_URI.erase(0, len);
		}
		else
		{
			_path.push_back(input_URI.substr(0, delimiter_slash_position));
			input_URI.erase(0, delimiter_slash_position + 1);
		}
	}
}

const void URIData::print_URI_data(void)
{
	std::vector<std::string>::iterator it = _path.begin();
	while(it != _path.end())
	{
		std::cout << *it << std::endl;
		it++;
	}
	std::cout << _query[0] << std::endl;
}



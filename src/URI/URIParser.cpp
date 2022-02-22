#include "URIParser.hpp"

#include <ctype.h>

#include "Utils.hpp"
#include "../HTTP/Exceptions/RequestException.hpp"

URIParser::URIParser(std::string URI_input)
{
	this->URI_input = URI_input;
}

URIParser::~URIParser(){};

static int hex_to_dec(char a)
{
	int dec_value;
	if(isalpha(a))
	{
		a = std::toupper(a);
		if(a <= 'F' && a >= 'A')
			dec_value = static_cast<int>(a) - 55;
		else
			dec_value = -1;
	}
	else if(isdigit(a))
		dec_value = a - 48;
	else
		dec_value = -1;
	return dec_value;
}

void URIParser::pct_decoding(std::string &target)
{
	int len = target.length();
	int decimal_value;
	std::string temp = target;
	target.erase(0, len);
	//TODO what if the %uj which is not % + HEX + HEX? do we throw an exception? 
	for(int i = 0; i < len; i++)
	{
		if(temp[i] == '%' && len > i + 2)
		{
			int first_dec = hex_to_dec(temp[i + 1]);
			int second_dec = hex_to_dec(temp[i + 2]);
			if(first_dec != -1 && second_dec != -1)
			{
				decimal_value = first_dec * 16 + second_dec;
				target += static_cast<char>(decimal_value);
				i += 2;
			}
			else
				throw Exception::RequestException(HTTPResponse::BadRequest);//TODO is this the write error code to throw in case the provided %hh is not qualified for decoding
		}
		else if(temp[i] == '%' && len <= (i + 2))
			throw Exception::RequestException(HTTPResponse::BadRequest);//TODO is this the write error code to throw in case the provided %hh is not qualified for decoding
		else
			target += temp[i];		
	}
}


void URIParser::parse(void)
{
	if (URI_input.size() > 2000)
		throw Exception::RequestException(HTTPResponse::URITooLong);
	parse_path();
	parse_queries();
}

void URIParser::parse_path(void)
{
	std::vector<std::string> path;
	std::string line;
	size_t len = URI_input.length();
	/*if the path is empty, it should have "/" at least*/
	if(len == 0)
	{
		path.push_back("/");
		uri.set_path(path);
		return;
	}
	size_t delimiter_query_position = URI_input.find_first_of("?");
	if(delimiter_query_position != std::string::npos)
	{
		query_string = URI_input.substr(delimiter_query_position + 1, len - delimiter_query_position - 1);
		URI_input.erase(delimiter_query_position, len - delimiter_query_position);
	}
	while(URI_input.length() > 0)
	{
		size_t delimiter_slash_position = URI_input.find_first_of("/");
		if(delimiter_slash_position == std::string::npos)
		{
			len = URI_input.length();
			line = URI_input.substr(0, len);
			pct_decoding(line);
			path.push_back(line);
			URI_input.erase(0, len);
		}
		else
		{
			line = URI_input.substr(0, delimiter_slash_position);
			pct_decoding(line);
			path.push_back(line);
			URI_input.erase(0, delimiter_slash_position + 1);
		}
	}
	uri.set_path(path);
}

void URIParser::parse_queries(void)
{
	std::vector<std::string> query;
	std::map<std::string, std::string> query_map;
	size_t delimiter_equal_position;
	size_t len;
	if(query_string.size() > 0)
	{
		query = Utils::_split_line(query_string, '&');
		int size = query.size();
		for(int i = 0; i < size; i++)
		{
			delimiter_equal_position = query[i].find_first_of("=");
			if(delimiter_equal_position == std::string::npos)
				throw Exception::RequestException(HTTPResponse::PreconditionFailed);//TODO test in nginx to see if it is correct
			len = query[i].length();
			std::string key = query[i].substr(0, delimiter_equal_position);
			std::string value = query[i].substr(delimiter_equal_position + 1, len - delimiter_equal_position - 1);
			pct_decoding(key);
			pct_decoding(value);
			query_map.insert(std::make_pair(key, value));
		}
	}
	uri.set_query(query_map);
}

void URIParser::print_URI_data(void)
{
	std::vector<std::string> path = uri.get_path();
	for(std::vector<std::string>::const_iterator it = path.begin(); it != path.end(); it++)
	{
		std::cout << *it << std::endl;
	}
	std::map<std::string, std::string> query = uri.get_query();
	for(std::map<std::string, std::string>::const_iterator it2 = query.begin(); it2 != query.end(); it2++)
	{
		std::cout << it2->first << "=" << it2->second << std::endl;
	}
}


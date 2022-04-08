#include "URIData.hpp"

namespace HTTPRequest {

	URIData::URIData(){}

	URIData::~URIData(){}

	void URIData::set_path(std::vector<std::string> path)
	{
		_path = path;
	}

	URIData::URIData(const URIData &other)
	{
		*this = other;
	}


	const URIData &URIData::operator=(const URIData &other)
	{
		_path = other._path;
		_query = other._query;
		return *this;
	}


	// void URIData::set_query(std::map<std::string, std::string> &query)
	// {
	// 	_query = query;
	// }
	void URIData::set_query(std::string &query)
	{
		_query = query;
	}

	const std::vector<std::string> URIData::get_path(void) const
	{
		return _path;
	}

	const std::string URIData::get_query(void) const
	{
		return _query;
	}


	void URIData::print_URI_data(void) const
	{
		for(std::vector<std::string>::const_iterator it = _path.begin(); it != _path.end(); it++)
		{
			std::cout << *it << std::endl;
		}
		std::cout << "query is: " << _query << std::endl;
		// for(std::map<std::string, std::string>::const_iterator it2 = _query.begin(); it2 != _query.end(); it2++)
		// {
		// 	std::cout << it2->first << "=" << it2->second << std::endl;
		// }
	}
}

#include "URIData.hpp"

#include "Utils.hpp"

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
}


void URIData::set_query(std::map<std::string, std::string> query)
{
	_query = query;
}

const std::vector<std::string> URIData::get_path(void) const
{
	return _path;
}

const std::map<std::string, std::string> URIData::get_query(void) const
{
	return _query;
}

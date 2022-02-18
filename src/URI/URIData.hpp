#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>

/*
assumptions:
TODO:
parse query
decoding
check if anything is case insensative
*/
class URIData{
private:
	std::vector<std::string> _path;
	std::vector<std::string> _query;
	std::string input_URI;
	URIData();

public:
	URIData(std::string input_URI);
	~URIData();
	void set_path(void);
	// void set_query(void);
	const std::vector<std::string> get_path(void) const;
	// const std::string get_query(void) const;
	const void print_URI_data(void);
};
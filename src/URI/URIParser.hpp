#pragma once

#include <iostream>
#include <string>

#include "URIData.hpp"

class URIParser{
private:
	URIData uri;
	std::string URI_input;
	std::string query_string;

	void pct_decoding(std::string &target);
	void parse_path(void);
	void parse_queries(void);
	URIParser();

public:
	URIParser(std::string URI_input);
	~URIParser();
	void parse(void);
	void print_URI_data();
	const URIData &get_uri_data(void) const;
};

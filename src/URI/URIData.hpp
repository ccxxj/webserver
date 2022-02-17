#pragma once

#include <iostream>
#include <string>
#include <map>

/*
assumptions:
the input uri is decoded in case there is percentage sign
the scheme characters are removed (including "://" e.g.http://google.com) when passed as input
*/
class URIData{
private:
	std::string _scheme;
	std::map<std::string, std::string> _authority;
	std::string _path;
	std::string _query;
	std::string _fragment;
	std::string input_URI;
	URIData();

public:
	URIData(std::string input_URI);
	~URIData();
	// void set_scheme(void);
	void set_authority(void);
	// void set_path(void);
	// void set_query(void);
	// void set_fragment(void);
	// const std::string get_scheme(void) const; //TODO convert to 3 get? (userinfo, host, name)
	const std::map<std::string, std::string> get_authority(void) const;
	// const std::string get_path(void) const;
	// const std::string get_query(void) const;
	// const std::string get_fragment(void) const;
	const void print_URI_data(void);
};
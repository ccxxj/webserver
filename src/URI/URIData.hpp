#pragma once

#include <iostream>
#include <string>

class URIData{
private:
	std::string _scheme;
	std::string _authority;
	std::string _path;
	std::string _query;
	std::string _fragment;
	URIData();

public:
	URIData(std::string input_URI);
	~URIData();
	void set_scheme(std::string input_URI);
	void set_authority(std::string input_URI);
	void set_path(std::string input_URI);
	void set_query(std::string input_URI);
	void set_fragment(std::string input_URI);
	const std::string get_scheme(void) const;
	const std::string get_authority(void) const;
	const std::string get_path(void) const;
	const std::string get_query(void) const;
	const std::string get_fragment(void) const;
};

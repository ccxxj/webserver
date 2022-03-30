#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>


namespace HTTPRequest {

	class URIData{
	private:
		std::vector<std::string> _path;
		std::string _query;
		// std::map<std::string, std::string> _query;

	public:
		URIData();
		~URIData();
		URIData(const URIData &other);
		const URIData &operator=(const URIData &other);

		void set_path(std::vector<std::string> path);
		// void set_query(std::map<std::string, std::string> &query);
		void set_query(std::string &query);
		const std::vector<std::string> get_path(void) const;
		// const std::map<std::string, std::string> get_query(void) const;
		const std::string get_query(void) const;
		const URIData &get_uri_data(void) const;
		void print_URI_data(void) const;
	};
}

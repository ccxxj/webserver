#pragma once

#include <iostream>
#include <string>

#include "URIData.hpp"

namespace HTTPRequest {

	class URIParser{
	private:
		std::string URI_input;
		std::string query_string;

		void pct_decoding(std::string &target);
		void parse_path(URIData &uri);
		void parse_queries(URIData &uri);
		URIParser();

	public:
		URIParser(std::string URI_input);
		~URIParser();
		void parse(URIData &uri);
	};
}

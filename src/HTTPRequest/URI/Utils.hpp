#pragma once

#include <iostream>
#include <vector>
#include <string>


namespace HTTPRequest {

	class Utils{
	public:
		static std::vector<std::string> _split_line(const std::string& line, const char delimiter);

	};
}

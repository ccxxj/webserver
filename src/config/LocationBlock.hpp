#pragma once

#include <vector>
#include <string>
#include "AConfigBlock.hpp"

namespace Config
{

	class LocationBlock : public AConfigBlock
	{
	private:
		int _autoindex;
		std::string _route;
		std::vector<std::string> _limit_except;
		
	public:
		LocationBlock();
		LocationBlock(const LocationBlock &other);
		const LocationBlock &operator=(const LocationBlock &other);
		~LocationBlock();

		void set_route(std::string str);
		void set_limit_except(std::string str);
		void set_autoindex(std::string str);
		int get_autoindex(void) const;
		const std::string& get_route(void) const;
		const std::vector<std::string>& get_limit_except(void) const;
	};
} // namespace Config

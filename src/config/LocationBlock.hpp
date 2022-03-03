#ifndef LocationBlock_HPP
#define LocationBlock_HPP
#include <vector>
#include <string>
#include "AConfigBlock.hpp"

namespace Config
{

	class LocationBlock : public AConfigBlock
	{
	private:
		/* data */
		std::string _route;
		std::vector<std::string> _limit_except;
		int _autoindex;

	public:
		LocationBlock(/* args */);
		LocationBlock(const LocationBlock &other);
		const LocationBlock &operator=(const LocationBlock &other);
		~LocationBlock();
		void set_route(std::string str);
		void set_limit_except(std::string str);
		void set_autoindex(std::string str);
		const std::string& get_route(void) const;
		int get_autoindex(void) const;
		const std::vector<std::string>& get_limit_except(void) const;
	};
} // namespace Config
#endif

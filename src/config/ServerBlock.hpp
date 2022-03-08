#pragma once

#include "AConfigBlock.hpp"
#include "LocationBlock.hpp"
#include <vector>
#include <set>
#include <string>

namespace Config
{

	class ServerBlock : public AConfigBlock
	{
	private:
		bool _is_default;
		std::set<std::string> _listen;
		std::vector<std::string> _server_name;
		std::vector<LocationBlock> _locations;
		
		/* check methods */
		std::string _check_and_return_port(const std::string& str);
		void _check_port_range(const std::string& port);

	public:
		ServerBlock();
		ServerBlock(const ServerBlock &other);
		~ServerBlock();
		const ServerBlock &operator=(const ServerBlock &other);

		/* getters & setters */
		void set_default(bool value);
		void set_listen(std::string str);
		void set_server_name(std::string str);
		void set_a_location(const LocationBlock &location);
		bool get_default(void) const;
		const std::set<std::string> &get_listen(void) const;
		const std::vector<std::string> &get_server_name(void) const;
		const std::vector<LocationBlock> &get_location(void) const;
	};
} // namespace Config

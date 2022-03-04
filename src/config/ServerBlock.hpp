#pragma once

#include "AConfigBlock.hpp"
#include "LocationBlock.hpp"
#include <vector>
#include <string>

namespace Config
{

	class ServerBlock : public AConfigBlock
	{
	private:
		bool _is_default;
		int _client_max_body_size;
		std::vector<std::string> _listen;
		std::vector<std::string> _server_name;
		std::vector<LocationBlock> _locations;

	public:
		ServerBlock();
		ServerBlock(const ServerBlock &other);
		~ServerBlock();
		const ServerBlock &operator=(const ServerBlock &other);

		void set_default(bool value);
		void set_listen(std::string str);
		void set_server_name(std::string str);
		void set_client_max_body_size(std::string str);
		void set_a_location(const LocationBlock &location);
		int get_client_max_body_size(void) const;
		bool get_default(void) const;
		const std::vector<std::string> &get_listen(void) const;
		const std::vector<std::string> &get_server_name(void) const;
		const std::vector<LocationBlock> &get_location(void) const;
	};
} // namespace Config

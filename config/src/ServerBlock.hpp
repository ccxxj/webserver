#ifndef ServerBlock_HPP
#define ServerBlock_HPP
#include "AConfigBlock.hpp"
#include "LocationBlock.hpp"
#include <vector>
#include <string>

namespace Config
{

	class ServerBlock : public AConfigBlock
	{
	private:
		/* data */
		bool _is_default;
		int _client_max_body_size;
		std::vector<std::string> _listen;
		std::vector<std::string> _server_name;
		std::vector<LocationBlock> _locations;

	public:
		ServerBlock(/* args */);
		ServerBlock(const ServerBlock &other);
		~ServerBlock();
		const ServerBlock &operator=(const ServerBlock &other);

		void set_default(bool value);
		void set_listen(std::string str);
		void set_server_name(std::string str);
		void set_client_max_body_size(std::string str);
		int get_client_max_body_size(void) const;
		bool get_default(void) const;
		std::vector<std::string> get_listen(void) const;
		std::vector<std::string> get_server_name(void) const;
		std::vector<LocationBlock> &get_location(void); //TODO have it with const? if we have it with const, we're not able to push things inside!
	};
} // namespace Config
#endif

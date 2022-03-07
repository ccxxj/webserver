#ifndef ConfigData_HPP
#define ConfigData_HPP

#include <vector>
#include <iostream>
#include "ServerBlock.hpp"

namespace Config
{

	class ConfigData
	{
	private:
		/* data */
		std::vector<ServerBlock> _servers;

		/* methods */
		// void _check_ports(void);
		// void _check_port_range(std::vector<std::string> ports);

	public:
		ConfigData(/* args */);
		~ConfigData();
		ConfigData(const ConfigData &other);
		const ConfigData &operator=(const ConfigData &other);

		/* check methods */
		void check_parsed_data(void);

		/* general methods */
		void make_first_server_default();
		void set_a_server(const ServerBlock &server);
		const std::vector<ServerBlock> &get_servers(void) const;

		/* print methods */
		void print_servers_info(void);
		void print_listen_info(ServerBlock &server);
		void print_server_name(ServerBlock &server);
		void print_root(ServerBlock &server);
		void print_returns(ServerBlock &server);
		void print_error_pages(ServerBlock &server);
		void print_multiple_locations_info(ServerBlock &server);
		void print_limit_except(LocationBlock &location);
	};
} // namespace Config
#endif

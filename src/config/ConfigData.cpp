#include "ConfigData.hpp"
#define B_RED "\033[1;31m"
#define MAGENTA "\033[36m"
#define PURPLE "\033[35m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

namespace Config
{

    ConfigData::ConfigData(/* args */)
    {
        // std::cout << "ConfigData default construtor" << std::endl;
    }

    ConfigData::ConfigData(const ConfigData &other)
    {
        // std::cout << "ConfigData copy construtor" << std::endl;
        *this = other;
    }

    const ConfigData &ConfigData::operator=(const ConfigData &other)
    {
        // std::cout << "ConfigData assign operator" << std::endl;
        _servers = other._servers;
        return *this;
    }

    ConfigData::~ConfigData()
    {
        // std::cout << "ConfigData destructor" << std::endl;
    }

    void ConfigData::make_first_server_default(void)
    {
        if (_servers.size())
            _servers[0].set_default(true);
    }

	std::vector<ServerBlock> &ConfigData::get_servers(void)
	{
		return (_servers);
	}

	void ConfigData::check_parsed_data(void)
	{
		if (_servers.size() == 0)
			throw std::runtime_error("Invalid-Config: empty file");
		else
		{
			_check_ports();
		}
	}

	void ConfigData::_check_ports(void)
	{
		std::vector<std::string> ports;
		for (size_t i = 0; i < _servers.size(); i++)
		{
			ports = _servers[i].get_listen();
			if(ports.size() == 0)
				throw std::runtime_error("Invalid-Config: listen line"); //TODO what if the second listen line is empty
			else
				_check_port_range(_servers[i].get_listen());
		}

	}

    void ConfigData::_check_port_range(std::vector<std::string> ports)
	{
        size_t port_num;
		std::string ipv6 = "[::]:";
		std::string str;
		for (size_t i = 0; i < ports.size(); i++)
		{
			size_t pos = ports[i].find(ipv6);
			if (pos != std::string::npos)
				ports[i] = ports[i].substr(pos + ipv6.length());
            port_num = std::atoi(ports[i].c_str());
            if (port_num < 1 || port_num > 65535)
                throw std::runtime_error("Listening port out of range or invalid");
		}
	}

	void ConfigData::print_listen_info(ServerBlock &server)
    {
        std::vector<std::string> ports = server.get_listen();
        std::cout << GREEN << "listening ports: ";
        for (size_t i = 0; i < ports.size(); i++)
        {
            std::cout << i + 1 << ")" << ports[i] << " ";
        }
        std::cout << RESET << std::endl;
    }

    void ConfigData::print_root(ServerBlock &server)
    {

        std::string root = server.get_root();
        std::cout << YELLOW << "root: " << root << RESET << std::endl;
    }

    void ConfigData::print_returns(ServerBlock &server)
    {

        std::vector<std::string> returns = server.get_return();
        std::cout << PURPLE << "returns: ";
        for (size_t i = 0; i < returns.size(); i++)
        {
            std::cout << i + 1 << ")" << returns[i] << " ";
        }
        std::cout << RESET << std::endl;
    }

    void ConfigData::print_error_pages(ServerBlock &server)
    {

        std::vector<std::string> error_pages = server.get_error_page();
        std::cout << "\033[34m"
                  << "error_pages: ";
        for (size_t i = 0; i < error_pages.size(); i++)
        {
            std::cout << i + 1 << ")" << error_pages[i] << " ";
        }
        std::cout << RESET << std::endl;
    }

    void ConfigData::print_server_name(ServerBlock &server)
    {

        std::vector<std::string> names = server.get_server_name();
        std::cout << MAGENTA << "server_names: ";
        for (size_t i = 0; i < names.size(); i++)
        {
            std::cout << i + 1 << ")" << names[i] << " ";
        }
        std::cout << RESET << std::endl;
    }

    void ConfigData::print_limit_except(LocationBlock &location)
    {

        std::vector<std::string> limits = location.get_limit_except();
        std::cout << MAGENTA << "\tmethods: ";
        for (size_t i = 0; i < limits.size(); i++)
        {
            std::cout << i + 1 << ")" << limits[i] << " ";
        }
        std::cout << RESET << std::endl;
    }

    void ConfigData::print_multiple_locations_info(ServerBlock &server)
    {
        std::vector<LocationBlock> locations = server.get_location();
        for (size_t i = 0; i < locations.size(); i++)
        {
            std::cout << RED << "\n\tLocation Number " << i + 1 << RESET << std::endl;
            std::cout << GREEN << "\troute: " << locations[i].get_route() << std::endl;
            std::cout << BLUE << "\tauto_index: " << locations[i].get_autoindex() << RESET << std::endl;
            print_limit_except(locations[i]);
            std::cout << "\t";
            print_root((ServerBlock &)locations[i]);
            std::cout << "\t";
            print_returns((ServerBlock &)locations[i]);
            std::cout << "\t";
            print_error_pages((ServerBlock &)locations[i]);
        }
    }

    void ConfigData::print_servers_info(void)
    {
        for (size_t i = 0; i < _servers.size(); i++)
        {
            std::cout << B_RED << "\nServer Number " << i + 1;
            if (_servers[i].get_default())
                std::cout << " (default)";
            std::cout << RESET << std::endl;
            print_listen_info(_servers[i]);
            print_server_name(_servers[i]);
            print_root(_servers[i]);
            std::cout << GREEN << "client_max_body_size: " << _servers[i].get_client_max_body_size() << RESET << std::endl;
            print_returns(_servers[i]);
            print_error_pages(_servers[i]);
            print_multiple_locations_info(_servers[i]);
        }
    }

} // namespace Config

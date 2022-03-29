#include "ConfigData.hpp"
#include "../Constants.hpp"

namespace Config
{

    ConfigData::ConfigData() { }

    ConfigData::ConfigData(const ConfigData &other)
    {
        *this = other;
    }

    const ConfigData &ConfigData::operator=(const ConfigData &other)
    {
        _servers = other._servers;
        return *this;
    }

    ConfigData::~ConfigData() { }

    void ConfigData::make_first_server_default(void)
    {
        if (_servers.size())
            _servers[0].set_default(true);
    }

    void ConfigData::set_a_server(const ServerBlock &server)
    {
        _servers.push_back(server);
    }

    const std::vector<ServerBlock> &ConfigData::get_servers(void) const
	{
		return (_servers);
	}

	void ConfigData::check_parsed_data(void)
	{
		std::string tmp = "root wwww;";
		if (_servers.size() == 0)
			throw std::runtime_error("Invalid-Config: empty file");
        for (size_t i = 0; i < _servers.size(); i++)
        {
            if(_servers[i].get_listen().size() == 0)
                _servers[i].set_listen("listen 80;");
			if(_servers[i].get_root().empty())
				_servers[i].set_root_value(tmp); //default root value
        }
	}

	void ConfigData::print_listen_info(ServerBlock &server)
    {
        std::set<std::string> listen_set = server.get_listen();
        std::cout << GREEN << "listening ports: ";
		for (std::set<std::string>::iterator i = listen_set.begin(); i != listen_set.end(); i++)
				std::cout << "X)" << *i << " ";
        std::cout << RESET << std::endl;
    }

    void ConfigData::print_root(ServerBlock &server)
    {
        std::string root = server.get_root();
        std::cout << YELLOW << "root: " << root << RESET << std::endl;
    }

    void ConfigData::print_returns(ServerBlock &server)
    {

        std::map<int, std::string> returns = server.get_return();
        std::cout << PURPLE << "returns: ";
        for (size_t i = 0; i < returns.size(); i++)
        {
            std::cout << i + 1 << ")" << returns[i] << " ";
        }
        std::cout << RESET << std::endl;
    }

    void ConfigData::print_error_pages(ServerBlock &server)
    {

        std::map<int, std::string> error_pages = server.get_error_page();
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
            std::cout << GREEN << "\tclient_max_body_size: " << locations[i].get_client_max_body_size() << RESET << std::endl;
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

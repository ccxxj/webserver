#ifndef _CONFIGPARSER_HPP_
# define _CONFIGPARSER_HPP_

# include <string>
# include <fstream>
# include <iostream>
# include <vector>
// TODO do we parse # hashtags and ignore them? (remove comments)
class ConfigParser
{
private:
	/* data */
	std::string		file_path;
	std::ifstream	file_stream;

	std::vector<std::string> server_tokens;
	/* methods */
	void	tokenize_server_blocks( void );
	void	print_server_blocks( void );

public:
	ConfigParser(std::string file_path);
	~ConfigParser();

	void	parse( void );

	class FailedToOpenException : public std::exception
	{
	public:
		const char *what() const throw()
		{
			return "Configuration file failed to open";
		}
	};
};

#endif



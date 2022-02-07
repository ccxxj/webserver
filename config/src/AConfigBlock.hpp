#ifndef _ACONFIG_HPP_
# define _ACONFIG_HPP_

#include <iostream>
#include <string>
#include <vector>

class AConfigBlock
{
protected:
	/* data */
	std::vector<std::string> 	_return;
	std::vector<std::string>	_root;
	std::vector<std::string>	_error_page;
public:
	//TODO check if additional cononibal form need to apply, also if additional actions need to taken for constructor and destructor
	AConfigBlock();
	virtual ~AConfigBlock();
	/* getters & setters */
	void set_return_value(std::string str);
	void set_root_value(std::string str);
	void set_error_page_value(std::string str);
	std::vector<std::string> get_return(void) const;
	std::vector<std::string> get_root(void) const;
	std::vector<std::string> get_error_page(void) const;
};

#endif

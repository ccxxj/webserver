#include <iostream>

class AConfigBlock
{
protected:
	/* data */
	std::string	_return;
	std::string	_root;
	std::string	_error_page;
	std::string	_limit_except;
public:
	AConfigBlock(/* args */);
	virtual ~AConfigBlock();
	/* getters & setters */
};


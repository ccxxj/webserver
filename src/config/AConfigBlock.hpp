#ifndef AConfig_HPP_
#define AConfig_HPP_
#include <iostream>
#include <string>
#include <vector>
namespace Config
{

	class AConfigBlock
	{
	protected:
		/* data */
		std::string _root;
		std::vector<std::string> _return;
		std::vector<std::string> _error_page;

	public:
		AConfigBlock();
		AConfigBlock(const AConfigBlock &other);
		const AConfigBlock &operator=(const AConfigBlock &other);
		virtual ~AConfigBlock();
		void _check_return_syntax(std::string str);
		/* getters & setters */
		void set_return_value(std::string str);
		void set_root_value(std::string str);
		void set_error_page_value(std::string str);
		std::string get_root(void) const;
		std::vector<std::string> get_return(void) const;
		std::vector<std::string> get_error_page(void) const;
	};
} // namespace Config
#endif

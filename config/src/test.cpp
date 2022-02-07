// #include <iostream>
// #include <string>
// #include <vector>

// void split_value(std::string str, std::vector<std::string> &a)
// {
// 	int found = str.find_first_of(" 	;");
// 	int len = str.length();
// 	int i = 0;
// 	while(found < len && found != -1)
// 	{
// 		a.push_back(str.substr(i, found - i));
// 		i = str.find_first_not_of(" 	;", found + 1);
// 		found = str.find_first_of(" 	;", i);
//         if (str[found] == ';')
//         {
//             a.push_back(str.substr(i, found - i));
//             break;
//         }
// 	}
// }

// int main()
// {
//     std::vector<std::string> a;
//     std::string str;
//     getline(std::cin, str);
//     split_value(str, a);
//     int size = a.size();
//     for(int i = 0; i < size; i++)
//     {
//         std::cout << a[i] << std::endl;
//     }
// }

#include "AConfigBlock.hpp"
#include "Utils.hpp"

void print_vectors(std::vector<std::string> a)
{
    std::cout << "vector is: \n";
    int size = a.size();
    for(int i = 0; i < size; i++)
    {
        std::cout << a[i] << std::endl;
    }
}

int main()
{
    AConfigBlock a;
    a.set_error_page_value("sdfasf asdfsf ds    ;");
    print_vectors(a.get_error_page());
    a.set_return_value("return /sf/sd/dsasff/ 444;");
    print_vectors(a.get_return());
    a.set_root_value("root /sf/sd/dsasff/ 444;");
    print_vectors(a.get_root());
}
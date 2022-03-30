#pragma once

#include <string>
#include <map>

namespace Utility
{ 
    class MimeTypes
    {
    private:
        /* data */
        static const std::map<std::string, std::string> _mimes;
        static std::map<std::string, std::string> create_mime_type_map();
    
    public:
        MimeTypes(/* args */);
        ~MimeTypes();
        MimeTypes(const MimeTypes &src);
        MimeTypes& operator=(const MimeTypes &rhs);

        std::string get_mime_type(const std::string &file_path);
    };   
}

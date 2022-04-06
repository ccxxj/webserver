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
        static const std::map<std::string, std::string> _exts;
        static std::map<std::string, std::string> create_mime_type_map();
        static std::map<std::string, std::string> create_extension_map();
    
    public:
        MimeTypes(/* args */);
        ~MimeTypes();
        MimeTypes(const MimeTypes &src);
        MimeTypes& operator=(const MimeTypes &rhs);

        std::string get_mime_type(const std::string &file_path);
        std::string get_extension(const std::string &mime_type);
    };   
}


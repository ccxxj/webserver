#include "MimeTypes.hpp"
#include <iostream>

namespace Utility
{
    //taken from https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
    std::map<std::string, std::string> MimeTypes::create_mime_type_map()
    {
        std::map<std::string, std::string> mimes;

        mimes["abw"] = "application/x-abiword";
        mimes["arc"] = "application/octet-stream";
        mimes["avif"] = "image/avif";
        mimes["azw"] = "application/vnd.amazon.ebook";
        mimes["bin"] = "application/octet-stream";
        mimes["bmp"] = "image/bmp";
        mimes["bz"] = "application/x-bzip";
        mimes["bz2"] = "application/x-bzip2";
        mimes["cda"] = "application/x-cdf";
        mimes["csh"] = "application/x-csh";
        mimes["css"] = "text/css";
        mimes["csv"] = "text/csv";
        mimes["doc"] = "application/msword";
        mimes["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
        mimes["eot"] = "application/vnd.ms-fontobject";
        mimes["epub"] = "application/epub+zip";
        mimes["gz"] = "application/gzip";
        mimes["gif"] = "image/gif";
        mimes["html"] = "text/html";
        mimes["ico"] = "image/x-icon";
        mimes["ics"] = "text/calendar";
        mimes["jar"] = "application/java-archive";
        mimes["jpeg"] = "image/jpeg";
        mimes["jpg"] = "image/jpeg";
        mimes["js"] = "application/javascript";
        mimes["json"] = "application/json";
        mimes["jsonld"] = "application/ld+json";
        mimes["mjs"] = "text/javascript";
        mimes["mpkg"] = "application/vnd.apple.installer+xml";
        mimes["odp"] = "application/vnd.oasis.opendocument.presentation";
        mimes["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
        mimes["odt"] = "application/vnd.oasis.opendocument.text";
        mimes["ogx"] = "application/ogg";
        mimes["otf"] = "font/otf";
        mimes["png"] = "image/png";
        mimes["pdf"] = "application/pdf";
        mimes["php"] = "application/x-httpd-php";
        mimes["ppt"] = "application/vnd.ms-powerpoint";
        mimes["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
        mimes["rar"] = "application/x-rar-compressed";
        mimes["rtf"] = "application/rtf";
        mimes["sh"] = "application/x-sh";
        mimes["svg"] = "image/svg+xml";
        mimes["swf"] = "application/x-shockwave-flash";
        mimes["tar"] = "application/x-tar";
        mimes["tif"] = "image/tiff";
        mimes["tiff"] = "image/tiff";
        mimes["ts"] = "application/typescript";
        mimes["ttf"] = "font/ttf";
        mimes["txt"] = "text/plain";
        mimes["vsd"] = "application/vnd.visio";
        mimes["webp"] = "image/webp";
        mimes["woff"] = "font/woff";
        mimes["woff2"] = "font/woff2";
        mimes["xhtml"] = "application/xhtml+xml";
        mimes["xls"] = "application/vnd.ms-excel";
        mimes["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
        mimes["xml"] = "application/xml";
        mimes["xul"] = "application/vnd.mozilla.xul+xml";
        mimes["zip"] = "application/zip";
        mimes["7z"] = "application/x-7x-compressed";

        return mimes;
    }

    const std::map<std::string, std::string> MimeTypes::_mimes = MimeTypes::create_mime_type_map();

    std::string MimeTypes::get_mime_type(const std::string &file_path) {
        std::string file_ext = file_path.substr(file_path.find_last_of(".") + 1);
        std::map<std::string, std::string>::const_iterator	mime_type;
        mime_type = _mimes.find(file_ext);
        if (mime_type == _mimes.end())
            mime_type = _mimes.find("txt"); //the default for textual files
        return mime_type->second;
    }

    MimeTypes::MimeTypes() {}

    MimeTypes::~MimeTypes() {}

    MimeTypes::MimeTypes(const MimeTypes &src) {
        *this = src;
    }

    MimeTypes &MimeTypes::operator=(const MimeTypes &rhs) {
        return *this;
    }
}

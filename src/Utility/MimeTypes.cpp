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

    std::map<std::string, std::string> MimeTypes::create_extension_map()
    {
        std::map<std::string, std::string> ext;

        ext["application/x-abiword"] = "abw";
        ext["application/octet-stream"] = "arc";
        ext["image/avif"] = "avif";
        ext["application/vnd.amazon.ebook"] = "azw";
        ext["application/octet-stream"] = "bin";
        ext["image/bmp"] = "bmp";
        ext["application/x-bzip"] = "bz";
        ext["application/x-bzip2"] = "bz2";
        ext["application/x-cdf"] = "cda";
        ext["application/x-csh"] = "csh";
        ext["text/css"] = "css";
        ext["text/csv"] = "csv";
        ext["application/msword"] = "doc";
        ext["application/vnd.openxmlformats-officedocument.wordprocessingml.document"] = "docx";
        ext["application/vnd.ms-fontobject"] = "eot";
        ext["application/epub+zip"] = "epub";
        ext["application/gzip"] = "gz";
        ext["image/gif"] = "gif";
        ext["text/html"] = "html";
        ext["image/x-icon"] = "ico";
        ext["text/calendar"] = "ics";
        ext["application/java-archive"] = "jar";
        ext["image/jpeg"] = "jpeg";
        ext["application/javascript"] = "js";
        ext["application/json"] = "json";
        ext["application/ld+json"] = "jsonld";
        ext["text/javascript"] = "mjs";
        ext["application/vnd.apple.installer+xml"] = "mpkg";
        ext["application/vnd.oasis.opendocument.presentation"] = "odp";
        ext["application/vnd.oasis.opendocument.spreadsheet"] = "ods";
        ext["application/vnd.oasis.opendocument.text"] = "odt";
        ext["application/ogg"] = "ogx";
        ext["font/otf"] = "otf";
        ext["image/png"] = "png";
        ext["application/pdf"] = "pdf";
        ext["application/x-httpd-php"] = "php";
        ext["application/vnd.ms-powerpoint"] = "ppt";
        ext["application/vnd.openxmlformats-officedocument.presentationml.presentation"] = "pptx";
        ext["application/x-rar-compressed"] = "rar";
        ext["application/rtf"] = "rtf";
        ext["application/x-sh"] = "sh";
        ext["image/svg+xml"] = "svg";
        ext["application/x-shockwave-flash"] = "swf";
        ext["application/x-tar"] = "tar";
        ext["image/tiff"] = "tiff";
        ext["application/typescript"] = "ts";
        ext["font/tttf"] = "tf";
        ext["text/plain"] = "txt";
        ext["application/vnd.visio"] = "vsd";
        ext["image/webp"] = "webp";
        ext["font/woff"] = "woff";
        ext["font/woff2"] = "woff2";
        ext["application/xhtml+xml"] = "xhtml";
        ext["application/vnd.ms-excel"] = "xls";
        ext["application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"] = "xlsx";
        ext["application/xml"] = "xml";
        ext["application/vnd.mozilla.xul+xml"] = "xul";
        ext["application/zip"] = "zip";
        ext["application/x-7x-compressed"] = "7z";

        return ext;
    }

    const std::map<std::string, std::string> MimeTypes::_mimes = MimeTypes::create_mime_type_map();
    const std::map<std::string, std::string> MimeTypes::_exts = MimeTypes::create_extension_map();

    std::string MimeTypes::get_mime_type(const std::string &file_path) {
        if(file_path.empty())
            return "";
        std::string file_ext = file_path.substr(file_path.find_last_of(".") + 1);
        std::map<std::string, std::string>::const_iterator	mime_type;
        mime_type = _mimes.find(file_ext);
        if (mime_type == _mimes.end())
            mime_type = _mimes.find("txt"); //the default for textual files
        return mime_type->second;
    }

    std::string MimeTypes::get_extension(const std::string &mime_type) {
        if(mime_type.empty())
            return "";
        std::map<std::string, std::string>::const_iterator	extension;
        	extension = _exts.find(mime_type);
        if (extension == _exts.end())
            extension = _mimes.find("NotSupported"); //the default for textual files
        return extension->second;
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

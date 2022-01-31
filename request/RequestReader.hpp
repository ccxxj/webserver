#ifndef REQUESTREADER_HPP
#define REQUESTREADER_HPP

#include <string>
#include <iostream> // TODO: remove

// TODO: get this macro from config
#define MAX_SIZE_BODY 8000

class RequestReader
{
private:
	bool _is_end_of_request_line(char* current_string);
    bool _is_end_of_header_fields(char* current_string, std::string& accumulating_string, size_t acc_length);

    char* _read_request_line(char* buffer, std::string& result, size_t* result_length);

    std::string& read_HTTP_request(char* buffer, std::string& result);

public:
	RequestReader(/* args */);
	~RequestReader();
};


#endif

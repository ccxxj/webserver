#ifndef REQUESTREADER_HPP
#define REQUESTREADER_HPP

#include <string>
#include <iostream> // TODO: remove

// TODO: get this macro from config
#define MAX_SIZE_BODY 8000

class RequestReader
{
private:
	bool _is_whitespace(char *current_string);
	bool _is_end_of_line(char *current_string);
	bool _is_end_of_header_fields(char *current_string, std::string &accumulating_string);

    std::string& read_HTTP_request(char* buffer, std::string& result);

	static size_t length_counter;
	std::string _accumulator;

public:
	RequestReader();
	~RequestReader();

	std::string& get_accumulator();
	
	char* read_line(char* buffer);

};

#endif

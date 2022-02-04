#ifndef REQUESTREADER_HPP
#define REQUESTREADER_HPP

#include <string>
#include <iostream> // TODO: remove

// TODO: get this macro from config
#define MAX_SIZE_BODY 8000

class RequestReader
{
private:
	bool _is_end_of_line(char *current_string, char* message_end);

	static size_t _length_counter;

public:
	RequestReader();
	~RequestReader();
	
	bool _is_end_of_header_fields(char* current_string, char* message_end);
	std::string read_line(char** buffer, char* message_end) ;

};

#endif

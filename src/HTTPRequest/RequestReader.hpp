#pragma once

#include <string>
#include <iostream> // TODO: remove

// TODO: get this macro from config
#define MAX_SIZE_BODY 8000

namespace HTTPRequest {
	class RequestReader
	{
	private:
		std::string _accumulator;
		static size_t _length_counter;
		
		bool _is_end_of_line(char character);

	public:
		RequestReader();
		~RequestReader();

		std::string read_line(char* buffer, size_t bytes_read, size_t* bytes_parsed, bool* can_be_parsed);
	};
}

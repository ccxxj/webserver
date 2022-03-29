#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <set>
#include <iostream> // TODO: remove
#include <sys/types.h>// for ssize_t

#include "RequestReader.hpp"
#include "RequestMessage.hpp"
#include "../HTTPResponse/ResponseMessage.hpp"
#include "../HTTPResponse/StatusCodes.hpp"
#include "URI/URIParser.hpp"

namespace HTTPRequest {

    class RequestParser {

    private:
        enum State
        {
            REQUEST_LINE,
            HEADER,
            PAYLOAD,
            CHUNKED_PAYLOAD,
            TRAILER,
            FINISHED
        };

        enum MessageBodyLength
        {
            CHUNKED,
            CONTENT_LENGTH,
            NOT_FOUND
        };

        RequestReader _request_reader;
        State _current_parsing_state;
        MessageBodyLength _payload_length;

        ssize_t _chunk_size;
		size_t _decoded_body_length;
		std::string _decoded_body;

        void _handle_request_message_part(std::string& line);
        void _parse_request_line(std::string& line);
        void _parse_header(std::string& line);
        void _define_message_body_length();
        bool _has_header_field(const std::string& header_name);
        void _parse_transfer_encoding(std::string coding_names_list);
        int _set_content_length();
        ssize_t _find_chunked_encoding_position(std::vector<std::string> &encodings, size_t encodings_num);
        void _parse_payload(std::string &line);
        void _parse_trailer_header_fields(std::string &line);
        void _decode_chunked(std::string& line);
        void _set_chunk_size(std::string& line);
        void _assign_decoded_body_length_to_content_length();
		bool _is_last_chunk();
		void _remove_chunked_from_transfer_encoding();
		void _check_disallowed_trailer_header_fields();
		void _remove_trailer_from_existing_header_fields();

        bool _is_method_supported(const std::string &method);
        size_t _longest_method_size();
        std::string _convert_header_name_touppercase(std::string& header_name);
        void _throw_request_exception(HTTPResponse::StatusCode error_status);
        
        struct Dispatch {
            State parsing_state;
            void (RequestParser::*ptr)(std::string& line);
        };

        static Dispatch _dispatch_table[];


    public:
        HTTPRequest::RequestMessage* _http_request_message;
        HTTPResponse::ResponseMessage* _http_response_message;

        RequestParser(HTTPRequest::RequestMessage* http_request, HTTPResponse::ResponseMessage* http_response);
        RequestParser(const RequestParser& other);
        ~RequestParser();

        void parse_HTTP_request(char* buffer, size_t bytes_read);
        bool is_parsing_finished();
    };
}

#endif

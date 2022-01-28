#include <string>
#include <set>
#include <vector>
#include <iostream> // TODO: remove

class ParseRequest {

public:
    ParseRequest();
    ParseRequest(const ParseRequest& other);
    ~ParseRequest();
    const ParseRequest& operator=(const ParseRequest& other);

    std::string& read_HTTP_request(char* buffer, std::string& result);

    static std::string& parse_method(const std::string& message_buffer);
    static std::string& parse_request_uri(const std::string& message_buffer);
    static std::string& parse_HTTP_version(const std::string& message_buffer);
    static std::string& parse_headers(const std::string& message_buffer);
    static std::string& parse_message_body(const std::string& message_buffer);
private:
    const std::string _request_line;

    bool _is_end_of_request_line(char* current_string);
    bool _is_end_of_header_fields(char* current_string, std::string& accumulating_string, size_t acc_length);

    char* _read_request_line(char* buffer, std::string& result, size_t* result_length);
private:
    enum State {
        METHOD = 0,
        REQUEST_URI,
        HTTP_VERSION,
        HEADERS,
        MESSAGE_BODY,
        NUM_STATES
    };

    struct Dispatcher {
        enum State state;
        std::string& (*ptr)(const std::string& value);
    };

    static Dispatcher dispatch_table[];

    const std::set<std::string> _request_methods;

    const std::string& _parse_request_line(char* buffer);
};

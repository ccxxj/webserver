
# set the project name
project(webserver)


# add the executable
add_executable(webserver
src/Webserver.cpp
src/HTTPRequest/RequestReader.cpp
src/HTTPRequest/HTTPRequestMethods.cpp
src/HTTPRequest/RequestParser.cpp
src/HTTPRequest/RequestMessage.cpp
src/HTTP/RequestHandler.cpp
src/HTTP/Exceptions/RequestException.cpp
src/HTTP/Connection.cpp
src/HTTP/Server.cpp
src/HTTPResponse/ResponseMessage.cpp
src/HTTPResponse/StatusCodes.cpp
src/main.cpp

)
add_executable(client client.cpp)

add_executable(parser_tests 
tests/main_test.cpp
tests/catch_amalgamated.cpp
tests/requestParserTests.cpp
src/HTTPRequest/RequestReader.cpp
src/HTTPRequest/HTTPRequestMethods.cpp
src/HTTPRequest/RequestParser.cpp
src/HTTPRequest/RequestMessage.cpp
src/HTTP/RequestHandler.cpp
src/HTTP/Exceptions/RequestException.cpp
src/HTTP/Connection.cpp
src/HTTP/Server.cpp
src/HTTPResponse/ResponseMessage.cpp
src/HTTPResponse/StatusCodes.cpp
)
target_compile_options(parser_tests PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-std=c++14>)

add_compile_options(
       -Wall -Werror -Wextra -Wno-error=maybe-uninitialized
       $<$<CONFIG:RELEASE>:-Ofast>
       $<$<CONFIG:DEBUG>-O0>
       $<$<CONFIG:DEBUG>-g>
)
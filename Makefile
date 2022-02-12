# executable names
NAME = webserver
EXE = $(NAME)

# directories
SRC_DIR = src
BUILD_DIR = build
BUILD_PATH = $(addprefix $(BUILD_DIR)/, out)
# INC_DIR = include

HEADERS = HTTPRequest/RequestMessage.hpp \
	HTTPRequest/RequestParser.hpp \
	HTTPRequest/RequestReader.hpp \
	HTTPRequest/HTTPRequestMethods.hpp \
	HTTP/Connection.hpp \
	HTTP/Server.hpp \
	HTTP/Exceptions/ClientErrorException.hpp \
	HTTP/Exceptions/ServerErrorException.hpp \
	HTTP/Exceptions/RequestException.hpp \
	HTTP/RequestHandler.hpp \
	Webserver.hpp \
	HTTPResponse/StatusCodes.hpp \
	HTTPResponse/ResponseMessage.hpp


SRC = Webserver.cpp \
	HTTPRequest/RequestReader.cpp \
	HTTPRequest/HTTPRequestMethods.cpp \
	HTTPRequest/RequestParser.cpp \
	HTTPRequest/RequestMessage.cpp \
	HTTP/RequestHandler.cpp \
	HTTP/Exceptions/RequestException.cpp \
	HTTP/Connection.cpp \
	HTTP/Server.cpp \
	HTTPResponse/ResponseMessage.cpp \
	HTTPResponse/StatusCodes.cpp \
	main.cpp


CXXFLAGS = -Wall -Wextra -Werror -Wno-unused-value\
		#-I$(INC_DIR) \ 
		-std=c++98 -pedantic \
		-g -fsanitize=address -O3

# # tests
# ifdef tests
# 	EXE = $(addprefix $(NAME), _tests)
# 	SRC_DIR = tests
# 
# 	SRC = main_test.cpp
# 
# 	HEADERS = $(addprefix $(SRC_DIR)/, catch.hpp) 
# 	BUILD_PATH = $(addprefix $(BUILD_DIR)/, tests/out)
# 	CXXFLAGS = -I$(INC_DIR) -std=c++14 \
# 			-g -fsanitize=address
# endif

HEADERS := $(addprefix $(SRC_DIR)/,$(HEADERS))
OBJ = $(SRC:.cpp=.o)
CXX=clang++

.PHONY: all clean fclean re tests

all: $(EXE)

$(EXE): $(addprefix $(BUILD_PATH)/,$(OBJ))
	$(CXX) -o $(EXE) $(CXXFLAGS) $(addprefix $(BUILD_PATH)/,$(OBJ))

$(BUILD_PATH)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	mkdir -p ${dir $@}
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# tests:
# 	$(MAKE) tests=1 all

clean:
	rm -rf $(BUILD_DIR)

fclean:
	rm -rf $(BUILD_DIR) $(EXE)

re:
	$(MAKE) fclean
	$(MAKE) all

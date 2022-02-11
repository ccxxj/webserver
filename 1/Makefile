# executable names
NAME = tests
EXE = $(NAME)

# directories
SRC_DIR = ../src
BUILD_DIR = build
BUILD_PATH = $(addprefix $(BUILD_DIR)/, out)
# INC_DIR = include


CXXFLAGS = -Wall -Wextra -Werror \
		#-I$(INC_DIR) \ 
		CXXFLAGS = -std=c++14 \
		-g -fsanitize=address

# tests request parser
ifdef tests_request_parser

	EXE = $(addprefix $(NAME), _request_parser)

	SRC = main_test.cpp

	HEADERS = HTTPRequest/RequestMessage.hpp \
		HTTPRequest/RequestParser.hpp \
		HTTPRequest/RequestReader.hpp \
		HTTP/Connection.hpp \
		HTTP/Server.hpp \
		HTTP/Exceptions/ClientErrorException.hpp \
		HTTP/Exceptions/ServerErrorException.hpp \
		HTTP/Exceptions/RequestException.hpp \
		HTTPResponse/ResponseMessage.hpp

	
	SRC = main_test.cpp \
		HTTPRequest/RequestReader.cpp \
		HTTPRequest/RequestParser.cpp \
		HTTPRequest/RequestMessage.cpp \
		HTTP/Exceptions/RequestException.cpp
		
	TEST_SRC = main_test.cpp \
			requestParserTests.cpp

	CATCH_HEADER = $(addprefix $(SRC_DIR)/, catch.hpp) 

endif

HEADERS := $(addprefix $(SRC_DIR)/,$(HEADERS))
OBJ = $(SRC:.cpp=.o)
TEST_OBJ = $(TEST_SRC:.cpp=.o)
CXX=clang++

.PHONY: all clean fclean re

all: $(EXE)

$(EXE): $(addprefix $(BUILD_PATH)/,$(OBJ)) $(addprefix $(BUILD_PATH)/,$(TEST_OBJ))
	$(CXX) -o $(EXE) $(CXXFLAGS) $(addprefix $(BUILD_PATH)/,$(OBJ)) $(addprefix $(BUILD_PATH)/,$(TEST_OBJ))

$(BUILD_PATH)/%.o: $(SRC_DIR)/%.cpp $(TEST_SRC)/%.cpp $(HEADERS)
	mkdir -p ${dir $@}
	$(CXX) $(CXXFLAGS) -c -o $@ $<

tests_request_parser:
	$(MAKE) tests_request_parser=1 all

clean:
	rm -rf $(BUILD_DIR)

fclean:
	rm -rf $(BUILD_DIR) $(EXE)

re:
	$(MAKE) fclean
	$(MAKE) all

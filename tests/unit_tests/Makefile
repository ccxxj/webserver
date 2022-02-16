# executable names
NAME = unit_tests
EXE = $(NAME)

# directories
LIBWEBSERV_DIR = ../../
BUILD_DIR = build
BUILD_PATH = $(addprefix $(BUILD_DIR)/, out)
SRC = main_test.cpp \
	catch_amalgamated.cpp \
	request_parser_unit_tests/request_parser_tests.cpp
			
CATCH_HEADER = catch_amalgamated.hpp

CXXFLAGS = -Wall -Wextra -Werror -Wno-unused-value\
		-std=c++14 \
		-g -fsanitize=address

TEST_OBJ = $(SRC:.cpp=.o)

CXX=clang++

.PHONY: all clean fclean re tests_request_parser

all: $(EXE)

$(EXE): $(addprefix $(BUILD_PATH)/,$(TEST_OBJ))
	cd $(LIBWEBSERV_DIR) && make
	$(CXX) -o $(EXE) $(CXXFLAGS) $(addprefix $(BUILD_PATH)/,$(TEST_OBJ)) -L$(LIBWEBSERV_DIR) -lwebserv 

$(BUILD_PATH)/%.o: %.cpp $(CATCH_HEADER)
	mkdir -p ${dir $@}
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)
	cd $(LIBWEBSERV_DIR) && make clean

fclean:
	rm -rf $(BUILD_DIR) $(EXE)
	cd $(LIBWEBSERV_DIR) && make fclean
re:
	$(MAKE) fclean
	$(MAKE) all

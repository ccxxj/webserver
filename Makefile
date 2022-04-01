# executable names
NAME = webserver
EXE = $(NAME)

# directories
SRC_DIR = src
BUILD_DIR = build
BUILD_PATH = $(addprefix $(BUILD_DIR)/, out)

HEADERS = Webserver.hpp \
	HTTPRequest/RequestMessage.hpp \
	HTTPRequest/RequestParser.hpp \
	HTTPRequest/RequestReader.hpp \
	HTTPRequest/HTTPRequestMethods.hpp \
	HTTP/Connection.hpp \
	HTTP/RequestHandler.hpp \
	HTTP/RequestHandlerDelegate.hpp \
	HTTP/Server.hpp \
	HTTP/Exceptions/ClientErrorException.hpp \
	HTTP/Exceptions/ServerErrorException.hpp \
	HTTP/Exceptions/RequestException.hpp \
	HTTPResponse/StatusCodes.hpp \
	HTTPResponse/ResponseHandler.hpp \
	HTTPResponse/ResponseMessage.hpp \
	HTTPResponse/SpecifiedConfig.hpp \
	CGI/CGIHandler.hpp\
	Utility/Utility.hpp \
	Utility/SmartPointer.hpp \
	Utility/File.hpp \
	Utility/MimeTypes.hpp

SRC = Webserver.cpp \
	HTTPRequest/RequestReader.cpp \
	HTTPRequest/HTTPRequestMethods.cpp \
	HTTPRequest/RequestParser.cpp \
	HTTPRequest/RequestMessage.cpp \
	HTTPRequest/URI/URIData.cpp \
	HTTPRequest/URI/URIParser.cpp \
	HTTP/RequestHandler.cpp \
	HTTP/Exceptions/RequestException.cpp \
	HTTP/Connection.cpp \
	HTTP/Server.cpp \
	HTTPResponse/StatusCodes.cpp \
	HTTPResponse/ResponseHandler.cpp \
	HTTPResponse/ResponseMessage.cpp \
	HTTPResponse/SpecifiedConfig.cpp \
	config/ConfigParser.cpp \
	config/ConfigData.cpp \
	config/AConfigBlock.cpp \
	config/ServerBlock.cpp \
	config/LocationBlock.cpp \
	config/ConfigValidator.cpp \
	config/ConfigTokenizer.cpp \
	CGI/CGIHandler.cpp\
	Utility/Utility.cpp \
	Utility/File.cpp \
	Utility/MimeTypes.cpp

CXXFLAGS = -Wall -Wextra -Werror -Wno-unused-value -Wno-unused-parameter\
		-std=c++98 -pedantic \
		-g -fsanitize=address

HEADERS := $(addprefix $(SRC_DIR)/,$(HEADERS))
OBJ = $(SRC:.cpp=.o)
CXX= clang++

.PHONY: all clean fclean re tests

all: libwebserv.a $(EXE)


UNAME := $(shell uname)
ifeq ($(UNAME), Linux)

$(EXE): $(addprefix $(BUILD_PATH)/,main.o) libwebserv.a
	$(CXX) -o $(EXE) $(CXXFLAGS) $(addprefix $(BUILD_PATH)/,main.o) -L. -lwebserv -lkqueue

libwebserv.a: $(addprefix $(BUILD_PATH)/,$(OBJ))
		ar -crs libwebserv.a $(addprefix $(BUILD_PATH)/,$(OBJ))

$(BUILD_PATH)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	mkdir -p ${dir $@}
	$(CXX) $(CXXFLAGS) -D=_LINUX -c -o $@ $<

else

$(EXE): $(addprefix $(BUILD_PATH)/,main.o) libwebserv.a
	$(CXX) -o $(EXE) $(CXXFLAGS) $(addprefix $(BUILD_PATH)/,main.o) -L. -lwebserv

libwebserv.a: $(addprefix $(BUILD_PATH)/,$(OBJ))
		ar -crs libwebserv.a $(addprefix $(BUILD_PATH)/,$(OBJ))

$(BUILD_PATH)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	mkdir -p ${dir $@}
	$(CXX) $(CXXFLAGS) -c -o $@ $<

endif

$(BUILD_PATH)/main.o: $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/main.cpp -c -o $@

clean:
	rm -rf $(BUILD_DIR)

fclean:
	rm -rf $(BUILD_DIR) $(EXE) libwebserv.a

re:
	$(MAKE) fclean
	$(MAKE) all

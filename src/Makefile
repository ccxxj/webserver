# Directories

# Files
SRCS	=	main.cpp \
			Webserver.cpp \
			HTTPRequest/HTTPRequestMethods.cpp \
			HTTPRequest/RequestReader.cpp \
			HTTPRequest/RequestParser.cpp \
			HTTPRequest/RequestMessage.cpp \
			HTTP/Connection.cpp \
			HTTP/RequestHandler.cpp \
			HTTP/Server.cpp \
			HTTPResponse/ResponseMessage.cpp \
			HTTPResponse/StatusCodes.cpp \
			config/ConfigParser.cpp \
			config/ConfigData.cpp \
			config/AConfigBlock.cpp \
			config/ServerBlock.cpp \
			config/LocationBlock.cpp \
			config/Utils.cpp \

OBJS = $(SRCS:.cpp=.o)
#$(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

# Compiler options
CC = c++
FLAGS	= #-pedantic -std=c++98 #-g -fsanitize=address #-Wall -Wextra -Werror 
EXECUTABLE	= webserv

all:	$(EXECUTABLE)

$(EXECUTABLE):	$(OBJS)
	$(CC) $(FLAGS) -o $(EXECUTABLE) $(OBJS)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(EXECUTABLE)

re: fclean all

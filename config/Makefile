# Directories
SRC_DIR = src
OBJ_DIR = obj

# Files
SRCS	=	main.cpp \
			ConfigParser.cpp \
			ConfigData.cpp \
			Utils.cpp \
			AConfigBlock.cpp \
			ServerBlock.cpp \
			LocationBlock.cpp \

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

# Compiler options
CC = c++
FLAGS	= -Wall -Wextra -Werror -pedantic -std=c++98 -g -fsanitize=address
EXECUTABLE	= webserv

all:	$(EXECUTABLE)

$(EXECUTABLE):	$(OBJS)
	$(CC) $(FLAGS) -o $(EXECUTABLE) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(EXECUTABLE)

re: fclean all

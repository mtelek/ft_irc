NAME = irc.exe
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
SRC = main.cpp server.cpp commands.cpp commands2.cpp commands_helper.cpp \
		 nickname.cpp password.cpp private_message.cpp username.cpp sighandler.cpp
OBJ_DIR = .objs
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))
DEPS = server.hpp error_messages.hpp

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(FLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR)/%.o: %.cpp $(DEPS) | $(OBJ_DIR)
	$(CXX) $(FLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

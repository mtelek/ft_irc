NAME = irc.exe
CXX = c++
#FLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -fsanitize=leak -g
FLAGS = -Wall -Wextra -Werror -std=c++98
SRC = main.cpp server.cpp commands.cpp \
		  sighandler.cpp \
		 CMDS/join.cpp CMDS/topic.cpp CMDS/part.cpp CMDS/kick.cpp CMDS/mode.cpp \
		 CMDS/cmds_helper.cpp CMDS/quit.cpp CMDS/cap.cpp  CMDS/nickname.cpp CMDS/password.cpp \
		 CMDS/private_message.cpp CMDS/username.cpp CMDS/commands_helper.cpp

OBJ_DIR = .objs
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))
DEPS = server.hpp error_messages.hpp

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(FLAGS) -o $(NAME) $(OBJ)

#$(OBJ_DIR)/%.o: %.cpp $(DEPS) | $(OBJ_DIR)
#	$(CXX) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.cpp $(DEPS)
	@mkdir -p $(dir $@)
	$(CXX) $(FLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

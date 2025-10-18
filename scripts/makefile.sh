#!/bin/bash

# Generate Makefile
cat << EOF > Makefile
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

NAME = ${EXEC_NAME}

SRC = ${SRC_FILES}
OBJ = \$(SRC:.cpp=.o)

all: \$(NAME)

\$(NAME): \$(OBJ)
	\$(CXX) \$(CXXFLAGS) -o \$(NAME) \$(OBJ)

%.o: %.cpp
	\$(CXX) \$(CXXFLAGS) -c \$< -o \$@

clean:
	rm -f \$(OBJ)

fclean: clean
	rm -f \$(NAME)

re: fclean all

.PHONY: all clean fclean re
EOF


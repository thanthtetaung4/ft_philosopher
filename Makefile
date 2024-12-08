# Project Names
NAME = philosopher

# Compiler and Flags
CC = cc
# CFLAGS = -Wall -Wextra -Werror
CFLAGS =

# Source Paths
UTILS = ./src/utils

# Source Files
SRC = ./src/main.c $(UTILS)/general_utils.c $(UTILS)/print_utils.c

# Object Files
OBJ = $(SRC:.c=.o)
TEST_OBJ = $(TEST_SRC:.c=.o)

# All Target
all: $(NAME)
	@echo "\033[32m[ $(NAME) is ready for use]\033[0m"

# NAME Target
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

# Clean Target
clean:
	rm -f $(OBJ)
	@echo "\033[33m[Cleaned up]\033[0m"

# Full Clean Target
fclean: clean
	rm -f $(NAME)
	@echo "\033[33m[Cleaned up]\033[0m"

# Rebuild Target
re: fclean all

.PHONY: all clean fclean re

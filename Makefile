# Project Names
NAME = philosopher
TEST = philo_test

# Compiler and Flags
CC = cc
# CFLAGS = -Wall -Wextra -Werror
CFLAGS =

# Source Paths
UTILS = ./src/utils

# Source Files
SRC = ./src/main.c ./src/input_validation.c $(UTILS)/general_utils.c $(UTILS)/print_utils.c
TEST_SRC =  ./test/test.c

# Object Files
OBJ = $(SRC:.c=.o)
TEST_OBJ = $(TEST_SRC:.c=.o)

# All Target
all: $(NAME)
	@echo "\033[32m[ $(NAME) is ready for use]\033[0m"

test: $(TEST)
	@echo "\033[32m[ $(TEST) is ready for use]\033[0m"

# NAME Target
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

#Test Target
$(TEST): $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $(TEST) $(TEST_OBJ)

# Clean Target
clean:
	rm -f $(OBJ)
	rm -f $(TEST_OBJ)
	@echo "\033[33m[Cleaned up]\033[0m"

# Full Clean Target
fclean: clean
	rm -f $(NAME)
	rm -f $(TEST)
	@echo "\033[33m[Cleaned up]\033[0m"

# Rebuild Target
re: fclean all

.PHONY: all clean fclean re

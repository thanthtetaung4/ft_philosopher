NAME = philo
CC = cc
CFLAGS = -Wall -Werror -Wextra -g -fsanitize=thread


SRCS = ./src/philosophers.c ./src/ft_atoi_vali.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

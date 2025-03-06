NAME = philo
CC = cc
CFLAGS = -Wall -Werror -Wextra #-g -fsanitize=thread

SRCS = src/philosophers.c src/ft_atoi_vali.c \
		src/init_clean_up.c src/monitor_tasks.c src/philo_tasks.c \
		src/routines.c src/utils.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lpthread

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

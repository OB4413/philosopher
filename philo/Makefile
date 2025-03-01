NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror #-fsanitize=thread

SURCE = philo.c routin_and_monitor.c init_all_helpmain.c take_the_fork.c
OBJ = $(SURCE:.c=.o)

all: $(NAME) clean

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
.SECANDARY: $(OBJ)
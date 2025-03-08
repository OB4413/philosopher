NAME = philo_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror #-fsanitize=thread

SURCE = philo_bonus.c routin_and_monitor_bonus.c init_all_helpmain_bonus.c take_the_fork_bonus.c
OBJ = $(SURCE:.c=.o)

all: $(NAME)

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
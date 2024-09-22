CC=cc  
SAN = -g3 -fsanitize=thread
CFLAGS= -Wall -Werror -Wextra  $(SAN)

SRC= architect.c matrix.c philo.c routine_start.c time.c utils.c
OBJ= $(SRC:.c=.o)

NAME = philo
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)
fclean:clean
	rm -rf $(NAME)
re: fclean all

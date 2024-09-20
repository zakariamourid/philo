CC=cc  
SAN = -g3 -fsanitize=thread
CFLAGS= -Wall  #$(SAN)

SRC=$(wildcard *.c)
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
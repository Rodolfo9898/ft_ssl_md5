NAME = ft_ssl

CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c99

INCLUDES = -Iinclude
SRC = $(shell find src -name '*.c' | sort)
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

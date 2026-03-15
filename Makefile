NAME = ft_ssl

CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c99

INCLUDES = -Iinclude
SRC = \
	src/main.c \
	src/core/stdin_tokenize.c \
	src/core/stdin_tokenize_utils.c \
	src/core/stdin_commands.c \
	src/commands/digest/digest_command.c \
	src/commands/digest/digest_sources.c \
	src/commands/digest/digest_output.c \
	src/commands/digest/md5.c \
	src/commands/digest/md5_algo.c \
	src/commands/digest/sha256.c \
	src/commands/digest/sha256_algo.c \
	src/commands/digest/whirlpool.c \
	src/commands/digest/whirlpool_round.c \
	src/commands/digest/whirlpool_algo.c \
	src/commands/help_command.c \
	src/core/command_registry.c \
	src/core/io.c \
	src/core/output.c \
	src/core/output_extra.c \
	src/core/utils.c \
	src/core/memory.c
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

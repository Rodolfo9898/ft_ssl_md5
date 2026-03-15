NAME = ft_ssl

CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c99

INCLUDES = -Iinclude
OBJDIR = objects
SRC = \
	src/main.c \
	src/core/stdin/tokenize.c \
	src/core/stdin/tokenize_utils.c \
	src/core/stdin/commands.c \
	src/commands/pipeline/command.c \
	src/commands/pipeline/sources.c \
	src/commands/pipeline/string.c \
	src/commands/pipeline/options.c \
	src/commands/pipeline/output.c \
	src/commands/md5/command.c \
	src/commands/md5/algo.c \
	src/commands/sha256/command.c \
	src/commands/sha256/algo.c \
	src/commands/whirlpool/command.c \
	src/commands/whirlpool/round.c \
	src/commands/whirlpool/algo.c \
	src/commands/help_command.c \
	src/core/command_registry.c \
	src/core/io.c \
	src/core/output/base.c \
	src/core/output/extra.c \
	src/core/utils.c \
	src/core/memory.c
OBJ = $(patsubst src/%.c,$(OBJDIR)/%.o,$(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJDIR)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    sources.mk                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/11/22 15:25:15 by rperez-t          #+#    #+#              #
#    Updated: 2026/11/22 18:29:45 by rperez-t         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# main.c alone at the root (the only main() of both builds), then one level
# of folders with files only: digest/ what every digest command shares (the
# flags, stdin, -s, files, the output), bits/ the bitwise logic and the
# rotations, hash/ md5 and sha256, utils/ the
# command table, the buffers and the output, bonus/ the bonus part.
MAIN_SRCS = main.c
BITS_DIR = bits
DIGEST_DIR = digest
HASH_DIR = hash
UTILS_DIR = utils
BONUS_DIR = bonus

BITS_SRCS = logic.c rotate.c
DIGEST_SRCS = command.c print.c sources.c
HASH_SRCS = md5.c md5_rounds.c padding.c sha256.c sha256_rounds.c
UTILS_SRCS = buffer.c dispatch.c output.c

# The bonus part. Both builds compile every file: make bonus adds
# -D BONUS_MODE=1 (BONUS_FLAGS), without which these files are empty, main()
# prints the usage and the command table has no whirlpool, see
# include/core/defines.h.
BONUS_SRCS = stdin_bonus.c tokenize_bonus.c tokens_bonus.c \
			whirlpool_bonus.c whirlpool_block_bonus.c whirlpool_build_bonus.c \
			whirlpool_tables_bonus.c

SRC_FILES = $(MAIN_SRCS) \
			$(addprefix $(BITS_DIR)/,$(BITS_SRCS)) \
			$(addprefix $(DIGEST_DIR)/,$(DIGEST_SRCS)) \
			$(addprefix $(HASH_DIR)/,$(HASH_SRCS)) \
			$(addprefix $(UTILS_DIR)/,$(UTILS_SRCS)) \
			$(addprefix $(BONUS_DIR)/,$(BONUS_SRCS))

OBJ_FILES = $(addprefix $(OBJ_DIR)/,$(SRC_FILES:.c=.o))
OBJ_BONUS_FILES = $(addprefix $(OBJ_BONUS_DIR)/,$(SRC_FILES:.c=.o))
DEP_FILES = $(OBJ_FILES:.o=.d) $(OBJ_BONUS_FILES:.o=.d)

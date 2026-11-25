# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    config.mk                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/11/22 15:25:15 by rperez-t          #+#    #+#              #
#    Updated: 2026/11/22 18:29:45 by rperez-t         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ssl
NAME_BONUS = ft_ssl_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c99 -O2
BONUS_FLAGS = -D BONUS_MODE=1
DEBUG_FLAGS = -g -fsanitize=address
DEPFLAGS = -MMD -MP

SRC_DIR = ./sources
BUILD_DIR = ./build
# Each build has its own object folder, both in build/; the programs stay at
# the root.
OBJ_DIR = $(BUILD_DIR)/mandatory
OBJ_BONUS_DIR = $(BUILD_DIR)/bonus
LIBS_DIR = ./libs
DOCKER_IMAGE = ft_ssl-test

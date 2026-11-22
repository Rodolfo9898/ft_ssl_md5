# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    config.mk                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/11/22 15:25:15 by rperez-t          #+#    #+#              #
#    Updated: 2026/11/22 16:10:55 by rperez-t         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ssl
NAME_BONUS = ft_ssl_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c99
BONUS_FLAGS = -D BONUS_MODE=1
DEBUG_FLAGS = -g -fsanitize=address
DEPFLAGS = -MMD -MP

SRC_DIR = ./sources
OBJ_DIR = ./objects
OBJ_BONUS_DIR = ./objects_bonus
LIBS_DIR = ./libs
DOCKER_IMAGE = ft_ssl-test

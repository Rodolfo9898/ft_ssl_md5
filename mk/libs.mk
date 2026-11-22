# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    libs.mk                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/11/22 15:54:47 by rperez-t          #+#    #+#              #
#    Updated: 2026/11/22 16:10:55 by rperez-t         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# libft: the string, memory and output helpers, built by its own Makefile.
LIBFT = $(LIBS_DIR)/libft/
LIBFT_LIB = $(addprefix $(LIBFT),libft.a)
LIBFT_FILES = $(wildcard $(LIBFT)Makefile $(LIBFT)mk/*.mk \
	$(LIBFT)include/*.h $(LIBFT)include/core/*.h $(LIBFT)sources/*.c $(LIBFT)sources/*/*.c)

LIB_FILES = $(LIBFT_LIB)

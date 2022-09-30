# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/19 21:33:05 by ngragas           #+#    #+#              #
#    Updated: 2021/05/19 21:39:09 by ngragas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= cute_sound.o
SRC		= cute_sound.c
CFLAGS	= -Wall -Wextra -Ofast -march=native
HEADER	= cute_sound.h

ifeq ($(shell uname -s), Linux)
	CFLAGS += -I/usr/include/SDL2/
endif

ifeq ($(OS),Windows_NT)
	CFLAGS += -D _WIN32
endif

all: $(NAME)
$(NAME): $(HEADER)
clean:
	@$(RM) $(NAME)
fclean: clean
re: fclean all
.PHONY: all clean fclean re

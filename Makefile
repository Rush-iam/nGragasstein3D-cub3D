# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ngragas <ngragas@student.21-school.ru>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/01/30 19:52:31 by ngragas           #+#    #+#              #
#    Updated: 2021/03/01 23:54:13 by ngragas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D
CFLAGS = -Wall -Wextra -Werror -MMD -g -Ofast
SRC =	main.c			\
		parse.c			\
		parse_map.c		\
		hooks.c			\
		control.c		\
		raycasting.c	\
		walls.c			\
		objects.c		\
		draw_fills.c	\
		draw_figures.c	\
		draw_map.c		\
		utils.c			\
		demo_tools.c

OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))
DEP = $(OBJ:.o=.d)
SRC_DIR = sources/
OBJ_DIR = objects/

INC_DIR = includes/
CFLAGS += -I$(INC_DIR)

LIB = $(LIB_DIR)libft.a
LIB_DIR = libft/
CFLAGS += -I$(LIB_DIR)

MLX = $(LIB_DIR)libmlx.a
MLX_DIR = minilibx_opengl/
CFLAGS += -I$(MLX_DIR)

all:
	$(MAKE) $(NAME) -j8
bonus: all
$(LIB): FORCE
	$(MAKE) -C $(LIB_DIR)
$(MLX): FORCE
	$(MAKE) -C $(MLX_DIR)
$(NAME): $(LIB) $(MLX) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ -lft -L$(LIB_DIR) \
		-lmlx -framework OpenGL -framework AppKit \
		-L$(MLX_DIR)
$(OBJ): | $(OBJ_DIR)
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@
-include $(DEP)

clean:
	$(RM) $(OBJ)
	$(RM) $(DEP)
	$(RM)r $(OBJ_DIR)
	$(MAKE) -C $(LIB_DIR) $@
fclean: clean
	$(RM) $(NAME)
	$(RM) $(LIB)
re: fclean all

norm:
	@norminette $(SRC_DIR)* $(INC_DIR)*
.PHONY: FORCE all clean fclean re bonus norm
FORCE:

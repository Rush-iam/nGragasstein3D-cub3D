# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ngragas <ngragas@student.21-school.ru>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/01/30 19:52:31 by ngragas           #+#    #+#              #
#    Updated: 2021/03/04 15:45:43 by ngragas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D
CFLAGS = -Wall -Wextra -Werror -MMD -g -Ofast -march=native
SRC =	main.c			\
		parse.c			\
		parse_set.c		\
		parse_set_map.c	\
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

LIB = $(LIB_DIR)libft.a
LIB_DIR = libft/

MLX = $(MLX_DIR)libmlx.a
MLX_DIR = minilibx_opengl/

#MLX = $(LIB_DIR)libmlx.dylib
#MLX_DIR = minilibx_swift/

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
		-L$(MLX_DIR) -I$(MLX_DIR) -lz
$(OBJ): | $(OBJ_DIR)
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INC_DIR) -I$(LIB_DIR) -I$(MLX_DIR)
-include $(DEP)

clean:
	$(RM) $(OBJ)
	$(RM) $(DEP)
	$(RM)r $(OBJ_DIR)
	$(MAKE) -C $(LIB_DIR) $@
	$(MAKE) -C $(MLX_DIR) $@
fclean: clean
	$(RM) $(NAME)
	$(RM) $(LIB)
	$(RM) $(MLX)
re: fclean all

norm:
	@norminette $(SRC_DIR)* $(INC_DIR)*
.PHONY: FORCE all clean fclean re bonus norm
FORCE:

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ngragas <ngragas@student.21-school.ru>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/01/30 19:52:31 by ngragas           #+#    #+#              #
#    Updated: 2021/05/19 21:54:14 by ngragas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 	= 	cub3D
CFLAGS 	= 	-Wall -Wextra -Werror -MMD -g3 -Ofast -march=native #-fsanitize=address
SRC    :=		main			\
				control			\
				draw_walls		\
				objects			\
				parse			\
				parse_set		\
				parse_set_map	\
				raycasting		\
				utils

SRC_BONUS :=	control_borders	\
				control_extra	\
				demo_tools		\
				doors			\
				doors_extra		\
				draw_ceilfloor	\
				draw_ceilfloor_plain \
				draw_effects	\
				draw_figures	\
				draw_fills		\
				draw_hud		\
				draw_message	\
				draw_objects	\
				draw_pixels		\
				draw_map		\
				draw_texture_set \
				enemies			\
				enemy_logic		\
				free_resources	\
				hooks			\
				initialize		\
				initialize_images \
				image_utils		\
				load_files		\
				load_resources	\
				pathfind		\
				pathfind_bfs_grid \
				pathfind_construct \
				pickups			\
				sounds			\
				screenshot		\
				terminate		\
				weapons

SRC_DIR := sources/
INC_DIR := includes/
OBJ_DIR = objects/
SRC_BONUS_DIR = sources_bonus/

#SRC_DIR := $(SRC_BONUS_DIR)
#SRC += $(SRC_BONUS)
#SRC := $(addsuffix _bonus, $(SRC))

ifeq ($(filter bonus, $(MAKECMDGOALS)), bonus)
	SWITCH_CLEAN := $(addsuffix .o, $(SRC))
	SWITCH_CLEAN += $(addsuffix .d, $(SRC))
	SRC_DIR = $(SRC_BONUS_DIR)
	SRC += $(SRC_BONUS)
	SRC := $(addsuffix _bonus, $(SRC))
else
	SWITCH_CLEAN := $(addsuffix _bonus.o, $(SRC) $(SRC_BONUS))
	SWITCH_CLEAN += $(addsuffix _bonus.d, $(SRC) $(SRC_BONUS))
endif
SWITCH_CLEAN := $(addprefix $(OBJ_DIR), $(SWITCH_CLEAN))

SRC := $(addsuffix .c, $(SRC))
OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))
DEP = $(OBJ:.o=.d)

LIB_DIR = libft/
LIB = $(LIB_DIR)libft.a

MLX_DIR = minilibx_opengl/
MLX = $(MLX_DIR)libmlx.a

CUTE_SOUND_DIR = cute_sound/
CUTE_SOUND = $(CUTE_SOUND_DIR)cute_sound.o

all: switch_clean
	$(MAKE) $(NAME) -j8
bonus: $(NAME) switch_clean
$(LIB): FORCE
	$(MAKE) -C $(LIB_DIR)
$(MLX): FORCE
	$(MAKE) -C $(MLX_DIR) CFLAGS=-DSTRINGPUTX11\ -Ofast\ -g\ -Wno-deprecated
$(CUTE_SOUND): FORCE
	$(MAKE) -C $(CUTE_SOUND_DIR)
$(NAME): $(LIB) $(MLX) $(CUTE_SOUND) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ -lft -L$(LIB_DIR) \
		-lmlx -L$(MLX_DIR) -I$(MLX_DIR) -framework OpenGL -framework AppKit -lz \
		$(CUTE_SOUND) -I$(CUTE_SOUND_DIR) -framework AudioUnit
$(OBJ): | $(OBJ_DIR)
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
$(OBJ_DIR)%.o: $(SRC_DIR)%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@ \
		-I$(INC_DIR) -I$(LIB_DIR) -I$(MLX_DIR) -I$(CUTE_SOUND_DIR)
-include $(DEP)

switch_clean:
	@$(RM) $(SWITCH_CLEAN)
clean:
	$(RM) $(OBJ)
	$(RM) $(DEP)
	$(RM)r $(OBJ_DIR)
	$(MAKE) -C $(LIB_DIR) $@
	$(MAKE) -C $(MLX_DIR) $@
	$(MAKE) -C $(CUTE_SOUND_DIR) $@
fclean: clean
	$(RM) $(NAME)
	$(RM) $(LIB)
	$(RM) $(MLX)
	$(RM) $(CUTE_SOUND)
re: fclean all

norm:
	@norminette $(SRC_DIR)* $(INC_DIR)*
.PHONY: FORCE all clean fclean re bonus switch_clean norm
FORCE:

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
CC		=	clang
CFLAGS 	= 	-Wall -Wextra -Werror -MMD -g -Ofast -march=native #-fsanitize=address
SRC    :=		main			\
				control			\
				control_borders	\
				draw_walls		\
				initialize		\
				parse			\
				parse_set		\
				parse_set_map	\
				raycasting		\
				utils

SRC_BONUS :=	control_extra	\
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
				initialize_images \
				image_utils		\
				load_files		\
				load_resources	\
				objects			\
				pathfind		\
				pathfind_bfs_grid \
				pathfind_construct \
				pickups			\
				sounds			\
				screenshot		\
				terminate		\
				weapons			\
				os_adapters/put_image_to_window	\
				os_adapters/new_image

ifeq ($(OS),Windows_NT)
	NAME := $(addsuffix .exe, $(NAME))
endif
SRC_DIR := sources/
INC_DIR := includes/
OBJ_DIR = objects/
SRC_BONUS_DIR = sources_bonus/

ifeq ($(filter bonus, $(MAKECMDGOALS)), bonus)
	SWITCH_CLEAN := $(addsuffix .o, $(SRC))
	SRC_DIR = $(SRC_BONUS_DIR)
	SRC := $(addsuffix _bonus, $(SRC) $(SRC_BONUS))
else
	SWITCH_CLEAN := $(addsuffix _bonus.o, $(SRC) $(SRC_BONUS))
endif
SWITCH_CLEAN := $(addprefix $(OBJ_DIR), $(SWITCH_CLEAN) $(SWITCH_CLEAN:.o=.d))
RM_SWITCH_CLEAN	:= $(shell $(RM) $(SWITCH_CLEAN))

SRC := $(addsuffix .c, $(SRC))
OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))
DEP = $(OBJ:.o=.d)

LIBFT_DIR	= libft/
LIBFT		= $(LIBFT_DIR)libft.a

PLATFORM := $(shell uname -s)
ifeq ($(PLATFORM), Darwin)
	MLX_DIR = minilibx_opengl/
else
	MLX_DIR = minilibx_linux/
endif
MLX = $(MLX_DIR)libmlx.a

CUTE_PNG_DIR	= cute_png/
CUTE_PNG		= $(CUTE_PNG_DIR)cute_png.o
CUTE_SOUND_DIR	= cute_sound/
CUTE_SOUND		= $(CUTE_SOUND_DIR)cute_sound.o

LIBS		= $(LIBFT) $(MLX)
CPPFLAGS	= -I$(LIBFT_DIR)	-I$(MLX_DIR)	-I$(INC_DIR)
LDFLAGS		= -L$(LIBFT_DIR)	-L$(MLX_DIR)
LDLIBS		= -lft				-lmlx

ifeq ($(PLATFORM), Darwin)
	LDLIBS	+= -framework OpenGL -framework AppKit -lz
else
	LDLIBS	+= -lX11 -lXext -lXrender -lm
	LIBS	+= $(CUTE_PNG)
	LDLIBS	+= $(CUTE_PNG)
	CPPFLAGS+= -I$(CUTE_PNG_DIR)
endif

ifeq ($(filter bonus, $(MAKECMDGOALS)), bonus)
	LIBS		+= $(CUTE_SOUND)
	LDLIBS		+= $(CUTE_SOUND)
	CPPFLAGS	+= -I$(CUTE_SOUND_DIR)
	ifeq ($(PLATFORM), Darwin)
		LDLIBS	+= -framework AudioUnit
	else ifeq ($(PLATFORM), Linux)
		LDLIBS	+= -lSDL2 -pthread
	else ifeq ($(OS),Windows_NT)
		LDLIBS	+= -lSDL2 -lpthread
	endif
endif

all bonus: $(NAME)
$(NAME): $(LIBS) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)
$(LIBFT): FORCE
	$(MAKE) -C $(LIBFT_DIR)
$(MLX): FORCE
	$(MAKE) -C $(MLX_DIR) "CFLAGS=-DSTRINGPUTX11 -Ofast \
		-Wno-deprecated -Wno-return-type -Wno-parentheses -Wno-pointer-sign"
$(CUTE_PNG): FORCE
	$(MAKE) -C $(CUTE_PNG_DIR)
$(CUTE_SOUND): FORCE
	$(MAKE) -C $(CUTE_SOUND_DIR)

$(OBJ): | $(OBJ_DIR)os_adapters
$(OBJ_DIR)os_adapters:
	mkdir -p $@
$(OBJ_DIR)%.o: $(SRC_DIR)%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@ $(CPPFLAGS)
-include $(DEP)

clean:
	$(RM) $(OBJ)
	$(RM) $(DEP)
	$(RM)r $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) $@
	$(MAKE) -C $(MLX_DIR) $@
	$(MAKE) -C $(CUTE_PNG_DIR) $@
	$(MAKE) -C $(CUTE_SOUND_DIR) $@
fclean: clean
	$(RM) $(NAME)
	$(RM) $(LIBFT)
	$(RM) $(MLX)
	$(RM) $(CUTE_PNG)
	$(RM) $(CUTE_SOUND)
re: fclean all

norm:
	@norminette $(SRC_DIR)* $(INC_DIR)*
.PHONY: FORCE all clean fclean re bonus switch_clean norm
FORCE:

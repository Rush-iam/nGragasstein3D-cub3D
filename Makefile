# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ngragas <ngragas@student.21-school.ru>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/01/30 19:52:31 by ngragas           #+#    #+#              #
#    Updated: 2021/02/04 22:46:49 by ngragas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D
CFLAGS = -Wall -Wextra -Werror -MMD -O2
SRC = main.c
OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))
DEP = $(OBJ:.o=.d)
LIB = $(LIB_DIR)libft.a
INCL_DIR = includes/
SRC_DIR = sources/
OBJ_DIR = objects/
LIB_DIR = libft/

all:
	$(MAKE) $(NAME) -j8
run: all
	./cub3D
bonus: all
$(LIB): FORCE
	$(MAKE) -C $(LIB_DIR)
$(NAME): $(LIB) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -I$(INCL_DIR) -o $@ \
		-lft -L$(LIB_DIR) -I$(LIB_DIR) -lmlx -framework OpenGL -framework AppKit
$(OBJ): | $(OBJ_DIR)
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCL_DIR) -I$(LIB_DIR)
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
	@norminette $(SRC_DIR)* $(INCL_DIR)*
.PHONY: FORCE all clean fclean re bonus norm
FORCE:

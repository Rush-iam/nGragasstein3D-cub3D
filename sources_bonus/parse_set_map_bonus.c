/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_set_map_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:49 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/08 18:06:22 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	set_map(t_game *game, t_list *map)
{
	unsigned	i;
	char		*line;
	unsigned	line_len;

	i = game->map.size.y;
	while (i > 0)
	{
		i--;
		if ((game->map.grid[i] = malloc(game->map.size.x)) == NULL)
			terminate(game, ERR_MEM, "Memory allocation failed (map cols)");
		line = ft_lstpop(&map);
		line_len = ft_strlen(line);
		ft_memcpy(game->map.grid[i], line, line_len);
		ft_memset(game->map.grid[i] + line_len, ' ',
					game->map.size.x - line_len);
		free(line);
	}
	set_map_process(game);
	if (game->p.pos.x == 0)
		terminate(game, ERR_PARSE, "Map player character 'NSWE' not found");
}

void	set_map_process(t_game *game)
{
	t_upoint	pt;
	const char	obj_chars[] = CHAR_OBJECTS;
	char		*chr;

	pt.y = 0;
	while (pt.y < game->map.size.y)
	{
		pt.x = 0;
		while (pt.x < game->map.size.x)
		{
			set_map_check_cell(game, game->map.grid, pt);
			if ((chr = ft_strchr(obj_chars, game->map.grid[pt.y][pt.x])))
			{
				if (*chr == 'n' || *chr == 's' || *chr == 'w' || *chr == 'e')
					set_map_object_add(game, *chr, sizeof(obj_chars) - 4, pt);
				else
					set_map_object_add(game, *chr, chr - obj_chars, pt);
			}
			pt.x++;
		}
		pt.y++;
	}
}

void	set_map_object_add(t_game *game, char chr, unsigned type, t_upoint pt)
{
	t_list		*obj_list;
	t_object	*obj;
	const char	*dirs = "eswn";

	if ((obj = ft_calloc(1, sizeof(t_object))) == NULL)
		terminate(game, ERR_MEM, "Memory allocation failed (object)");
	obj->pos = (t_fpoint){pt.x + 0.5, pt.y + 0.5};
	obj->sprite = &game->sprite[type];
	obj->type = type;
	if (ft_strchr(CHAR_SOLID, chr))
		obj->solid = true;
	if (obj->type == T_ENEMY)
	{
		obj->angle = M_PI_2 * (ft_strchr(dirs, chr) - dirs);
		if ((obj->e = ft_calloc(1, sizeof(*obj->e))) == NULL)
			terminate(game, ERR_MEM, "Memory allocation failed (enemy)");
		obj->e->health = 100;
	}
	if ((obj_list = ft_lstnew(obj)) == NULL)
		terminate(game, ERR_MEM, "Memory allocation failed (obj_list)");
	ft_lstadd_front(&game->objects, obj_list);
}

void	set_map_check_cell(t_game *game, char **map, t_upoint pt)
{
	const char	*dirs = "ESWN";

	if (ft_strchr(CHAR_ALLOWED, map[pt.y][pt.x]) == NULL)
		terminate(game, ERR_PARSE, "Wrong map character.");
	if (map[pt.y][pt.x] == ' ' || ft_isdigit(map[pt.y][pt.x]))
		return ;
	if (pt.x == 0 || pt.x == game->map.size.x - 1 ||
		pt.y == 0 || pt.y == game->map.size.y - 1)
		terminate(game, ERR_PARSE, "Map must be closed/surrounded by walls");
	if (map[pt.y - 1][pt.x] == ' ' || map[pt.y + 1][pt.x] == ' ' ||
		map[pt.y][pt.x - 1] == ' ' || map[pt.y][pt.x + 1] == ' ')
		terminate(game, ERR_PARSE, "Map must be closed/surrounded by walls");
	if (ft_strchr(dirs, map[pt.y][pt.x]))
	{
		if (game->p.pos.x != 0)
			terminate(game, ERR_PARSE, "Duplicated map player character NSWE");
		game->p.pos = (t_fpoint){pt.x + 0.5, pt.y + 0.5};
		game->p.angle = M_PI_2 * (ft_strchr(dirs, map[pt.y][pt.x]) - dirs);
		game->map.grid[pt.y][pt.x] = '.';
	}
}

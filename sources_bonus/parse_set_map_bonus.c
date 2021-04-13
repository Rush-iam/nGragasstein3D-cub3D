/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_set_map_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:49 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/13 20:43:01 by ngragas          ###   ########.fr       */
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
	char		*chr_ptr;
	char		chr;

	pt.y = 0;
	while (pt.y < game->map.size.y)
	{
		pt.x = 0;
		while (pt.x < game->map.size.x)
		{
			set_map_check_cell(game, game->map.grid, pt);
			chr = game->map.grid[pt.y][pt.x];
			if ((chr_ptr = ft_strchr(obj_chars, chr)))
				set_map_object_add(game, chr, chr_ptr - obj_chars, pt);
			else if (chr == 'n' || chr == 's' || chr == 'w' || chr == 'e')
				set_map_object_add(game, chr, sizeof(CHAR_OBJECTS) - 1, pt);
			else if (ft_strchr(CHAR_DOORS, chr))
				set_map_door_add(game, pt);
			pt.x++;
		}
		pt.y++;
	}
}

void	set_map_object_add(t_game *game, char chr, unsigned type, t_upoint pt)
{
	t_object	*obj;
	const char	*dirs = "eswn";

	if ((obj = ft_calloc(1, sizeof(t_object))) == NULL)
		terminate(game, ERR_MEM, "Memory allocation failed (object)");
	obj->pos = (t_fpoint){pt.x + 0.5, pt.y + 0.5};
	if (type != sizeof(CHAR_OBJECTS) - 1)
		obj->sprite = &game->sprite[type];
	if (type > sizeof(CHAR_DECOR) - 2)
		obj->type = type - (sizeof(CHAR_DECOR) - 2);
	if (obj->type == T_ENEMY)
	{
		if ((obj->e = ft_calloc(1, sizeof(*obj->e))) == NULL)
			terminate(game, ERR_MEM, "Memory allocation failed (enemy)");
		obj->e->angle = M_PI_2 * (ft_strchr(dirs, chr) - dirs);
		obj->e->health = ENEMY_HEALTH;
		obj->e->type = ENEMY_GUARD;
		enemy_set_state(game, obj, ST_WAIT);
	}
	if (ft_strchr(CHAR_SOLID, chr))
		game->map.grid[pt.y][pt.x] = CHAR_SOLID_MAP;
	else
		game->map.grid[pt.y][pt.x] = '.';
	object_add(game, &game->objects, obj);
}

void	set_map_door_add(t_game *game, t_upoint pt)
{
	t_door	*door;

	if ((door = ft_calloc(1, sizeof(t_object))) == NULL)
		terminate(game, ERR_MEM, "Memory allocation failed (door)");
	door->cell = (t_point){pt.x, pt.y};
	object_add(game, &game->doors, door);
}

void	set_map_check_cell(t_game *game, char **map, t_upoint pt)
{
	const char	*dirs = "ESWN";

	if (ft_strchr(CHAR_ALLOWED, map[pt.y][pt.x]) == NULL)
		terminate(game, ERR_PARSE, "Wrong map character");
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

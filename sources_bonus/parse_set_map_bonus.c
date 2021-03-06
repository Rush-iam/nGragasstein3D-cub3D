/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_set_map_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:49 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/05 21:16:16 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	set_map(t_game *game, t_list *map)
{
	uint32_t	i;
	char		*line;
	uint32_t	line_len;

	game->map.grid = malloc(sizeof(*game->map.grid) * game->map.size.y);
	if (game->map.grid == NULL)
		terminate(game, ERR_MEM, "Memory allocation failed (map rows)");
	i = game->map.size.y;
	while (i > 0)
	{
		i--;
		game->map.grid[i] = malloc(game->map.size.x);
		if (game->map.grid[i] == NULL)
			terminate(game, ERR_MEM, "Memory allocation failed (map cols)");
		line = ft_lstpop(&map);
		line_len = ft_strlen(line);
		ft_memcpy(game->map.grid[i], line, line_len);
		ft_memset(game->map.grid[i] + line_len, ' ', \
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
	char		chr;

	pt.y = 0;
	while (pt.y < game->map.size.y)
	{
		pt.x = 0;
		while (pt.x < game->map.size.x)
		{
			set_map_check_cell(game, game->map.grid, pt);
			chr = game->map.grid[pt.y][pt.x];
			if (ft_strchr(CHAR_OBJECTS, chr))
				set_map_object_add(game, chr, CHAR_OBJECTS, pt);
			else if (ft_strchr(CHAR_ENEMY, chr))
				set_map_enemy_add(game, chr, CHAR_ENEMY, pt);
			else if (ft_strchr(CHAR_DOORS, chr) || chr == *CHAR_SECRET)
				set_map_door_add(game, pt);
			pt.x++;
		}
		pt.y++;
	}
}

void	set_map_object_add(t_game *game, char chr, char *set, t_upoint pt)
{
	t_object	*obj;
	uint32_t	type = ft_strchr(set, chr) - set;

	if (ft_assign_ptr((void **)&obj, ft_calloc(1, sizeof(*obj))) == NULL)
		terminate(game, ERR_MEM, "Memory allocation failed (object)");
	obj->pos = (t_fpoint){pt.x + 0.5f, pt.y + 0.5f};
	obj->sprite = &game->sprite[type];
	if (type > sizeof(CHAR_DECOR) - 2)
		obj->type = type - (sizeof(CHAR_DECOR) - 2);
	else
		obj->type = T_DECOR;
	if (ft_strchr(CHAR_SOLID, chr))
		game->map.grid[pt.y][pt.x] = CHAR_SOLID_MAP;
	else
		game->map.grid[pt.y][pt.x] = '.';
	object_add(game, &game->objects, obj);
}

void	set_map_enemy_add(t_game *game, char chr, char *set, t_upoint pt)
{
	t_object	*obj;

	if (ft_assign_ptr((void **)&obj, ft_calloc(1, sizeof(*obj))) == NULL)
		terminate(game, ERR_MEM, "Memory allocation failed (object)");
	obj->pos = (t_fpoint){pt.x + 0.5f, pt.y + 0.5f};
	obj->type = T_ENEMY;
	if (!ft_assign_ptr((void **)&obj->e, ft_calloc(1, sizeof(*obj->e))))
		terminate(game, ERR_MEM, "Memory allocation failed (enemy)");
	if (chr == *CHAR_ENEMY_DOG)
	{
		obj->e->type = ENEMY_DOG;
		obj->e->health = ENEMY_DOG_HEALTH;
		obj->e->speed = ENEMY_DOG_SPEED;
		enemy_melee_set_state(game, obj, ST_WALK);
	}
	else
	{
		obj->e->type = ENEMY_GUARD;
		obj->e->health = ENEMY_GUARD_HEALTH;
		obj->e->speed = ENEMY_GUARD_SPEED;
		enemy_range_set_state(game, obj, ST_WAIT);
		obj->e->location = obj->pos;
		obj->e->location_angle = obj->e->angle;
		obj->e->angle = M_PI_2_F * (ft_strchr(set, chr) - set);
	}
	game->map.grid[pt.y][pt.x] = '.';
	object_add(game, &game->objects, obj);
}

void	set_map_door_add(t_game *game, t_upoint pt)
{
	t_door	*door;

	door = ft_calloc(1, sizeof(*door));
	if (door == NULL)
		terminate(game, ERR_MEM, "Memory allocation failed (door)");
	door->cell = (t_point){pt.x, pt.y};
	if (game->map.grid[pt.y][pt.x] == *CHAR_SECRET)
	{
		door->secret = true;
		if (chr_is_wall(game->map.grid[pt.y - 1][pt.x]))
			door->secret_texture_id = game->map.grid[pt.y - 1][pt.x] - '0';
		else if (chr_is_wall(game->map.grid[pt.y + 1][pt.x]))
			door->secret_texture_id = game->map.grid[pt.y + 1][pt.x] - '0';
		else if (chr_is_wall(game->map.grid[pt.y][pt.x - 1]))
			door->secret_texture_id = game->map.grid[pt.y][pt.x - 1] - '0';
		else if (chr_is_wall(game->map.grid[pt.y][pt.x + 1]))
			door->secret_texture_id = game->map.grid[pt.y][pt.x + 1] - '0';
	}
	else if (game->map.grid[pt.y][pt.x] == *CHAR_ELEVATOR)
		door->end_level = true;
	object_add(game, &game->doors, door);
}

void	set_map_check_cell(t_game *game, char **map, t_upoint pt)
{
	const char	*dirs = "ESWN";

	if (ft_strchr(CHARS, map[pt.y][pt.x]) == NULL)
		terminate(game, ERR_PARSE, "Wrong map character");
	if (map[pt.y][pt.x] == ' ' || chr_is_wall(map[pt.y][pt.x]))
		return ;
	if (pt.x == 0 || pt.x == game->map.size.x - 1 || \
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

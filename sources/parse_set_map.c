/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_set_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/04 16:01:00 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/04 16:01:00 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
	t_list		*obj_list;
	t_object	*obj;

	pt.y = 0;
	while (pt.y < game->map.size.y)
	{
		pt.x = 0;
		while (pt.x < game->map.size.x)
		{
			set_map_check_cell(game, game->map.grid, pt);
			if (game->map.grid[pt.y][pt.x] == '2')
			{
				if ((obj = malloc(sizeof(t_object))) == NULL)
					terminate(game, ERR_MEM, "Memory allocation failed (obj)");
				*obj = (t_object){&game->texture[SPRITE],
					(t_fpoint){pt.x + 0.5, pt.y + 0.5}, (t_upoint){0, 0}, 0};
				if ((obj_list = ft_lstnew(obj)) == NULL)
					terminate(game, ERR_MEM, "Memory allocation failed (objl)");
				ft_lstadd_front(&game->objects, obj_list);
			}
			pt.x++;
		}
		pt.y++;
	}
}

void	set_map_check_cell(t_game *game, char **map, t_upoint pt)
{
	const char	*dirs = "ESWN";

	if (ft_strchr(" 012NSWE", map[pt.y][pt.x]) == NULL)
		terminate(game, ERR_PARSE, "Wrong map character. Allowed: 012NSWE");
	if (map[pt.y][pt.x] == ' ' || map[pt.y][pt.x] == '1')
		return ;
	if (pt.x == 0 || pt.x == game->map.size.x - 1 ||
		pt.y == 0 || pt.y == game->map.size.y - 1)
		terminate(game, ERR_PARSE, "Map must be closed/surrounded by walls");
	if (map[pt.y - 1][pt.x] == ' ' || map[pt.y + 1][pt.x] == ' ' ||
		map[pt.y][pt.x - 1] == ' ' || map[pt.y][pt.x + 1] == ' ')
		terminate(game, ERR_PARSE, "Map must be closed/surrounded by walls");
	if (map[pt.y][pt.x] == '0' || map[pt.y][pt.x] == '2')
		return ;
	else if (ft_strchr(dirs, map[pt.y][pt.x]))
	{
		if (game->p.pos.x != 0)
			terminate(game, ERR_PARSE, "Duplicated map player character");
		game->p.pos = (t_fpoint){pt.x + 0.5, pt.y + 0.5};
		game->p.angle = M_PI_2 * (ft_strchr(dirs, map[pt.y][pt.x]) - dirs);
		game->map.grid[pt.y][pt.x] = '0';
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 18:43:41 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/01 15:30:12 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	parse_map(int file_id, char *line, t_game *game)
{
	t_list		*map;
	int			status;
	unsigned	line_len;

	map = ft_lstnew(line);
	game->map.size = (t_upoint){ft_strlen(line), 1};
	while ((status = get_next_line(file_id, &line)) >= 0 && *line != '\0')
	{
		ft_lstadd_front(&map, ft_lstnew(line));
		if ((line_len = ft_strlen(line)) > game->map.size.x)
			game->map.size.x = line_len;
		game->map.size.y++;
	}
	if (status == -1)
		terminate(ERROR_PARSE, "Something went wrong while reading scene file");
	free(line);
	if (status != 0)
		terminate(ERROR_PARSE, "Empty lines in map content are not allowed");
	if ((game->map.grid = malloc(sizeof(char *) * game->map.size.y)) == NULL)
		terminate(ERROR_MEMORY, "Memory allocation failed (map rows)");
	set_map(game, map);
}

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
			terminate(ERROR_MEMORY, "Memory allocation failed (map cols)");
		line = ft_lstpop(&map);
		line_len = ft_strlen(line);
		ft_memcpy(game->map.grid[i], line, line_len);
		ft_memset(game->map.grid[i] + line_len, ' ',
					game->map.size.x - line_len);
		free(line);
	}
	set_map_process(game);
}

void	set_map_process(t_game *game)
{
	t_upoint	pt;
	t_object	*obj;

	pt.y = 0;
	while (pt.y < game->map.size.y)
	{
		pt.x = 0;
		while (pt.x < game->map.size.x)
		{
			if (ft_strchr(" 012NSWE", game->map.grid[pt.y][pt.x]) == NULL)
				terminate(ERROR_PARSE, "Wrong map character. Allowed: 012NSWE");
			if (game->map.grid[pt.y][pt.x] != ' ' &&
				game->map.grid[pt.y][pt.x] != '1')
				set_map_check_cell(game, game->map.grid, pt);
			if (game->map.grid[pt.y][pt.x] == '2')
			{
				obj = malloc(sizeof(t_object));
				*obj = (t_object){&game->texture[SPRITE],
								(t_fpoint){pt.x + 0.5, pt.y + 0.5}, 0, 0, 0};
				ft_lstadd_front(&game->objects, ft_lstnew(obj));
			}
			pt.x++;
		}
		pt.y++;
	}
}

void	set_map_check_cell(t_game *game, char **map, t_upoint pt)
{
	const char	*dirs = "ESWN";

	if (pt.x == 0 || pt.x == game->map.size.x - 1 ||
		pt.y == 0 || pt.y == game->map.size.y - 1)
		terminate(ERROR_PARSE, "Map must be closed/surrounded by walls");
	if (map[pt.y - 1][pt.x] == ' ' || map[pt.y + 1][pt.x] == ' ' ||
		map[pt.y][pt.x - 1] == ' ' || map[pt.y][pt.x + 1] == ' ')
		terminate(ERROR_PARSE, "Map must be closed/surrounded by walls");
	if (map[pt.y][pt.x] == '0' || map[pt.y][pt.x] == '2')
		return ;
	else if (ft_strchr(dirs, map[pt.y][pt.x]))
	{
		if (game->p.pos.x != 0)
			terminate(ERROR_PARSE, "Duplicated map player character");
		game->p.pos = (t_fpoint){pt.x + 0.5, pt.y + 0.5};
		game->p.angle = M_PI_2 * (ft_strchr(dirs, map[pt.y][pt.x]) - dirs);
		game->map.grid[pt.y][pt.x] = '0';
	}
}

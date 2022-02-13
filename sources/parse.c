/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 18:27:20 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/02 20:05:53 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	parse(char *filename, t_game *game)
{
	int		file_fd;
	char	*line;

	if (ft_strlen(filename) < 5 \
	|| ft_memcmp(".cub", filename + ft_strlen(filename) - 4, 5) != 0)
		terminate(game, ERR_ARGS, "Wrong scene filename");
	file_fd = open(filename, O_RDONLY);
	if (file_fd == -1)
		terminate(game, ERR_ARGS, strerror(errno));
	parse_scene(file_fd, &line, game);
	validate_settings(game);
	parse_map(file_fd, line, game);
	if (close(file_fd) == -1)
		terminate(game, ERR_PARSE, strerror(errno));
}

void	parse_scene(int file_fd, char **line, t_game *game)
{
	int	status;

	game->color_floor = -1U;
	game->color_ceil = -1U;
	status = get_next_line(file_fd, line);
	while (status >= 0)
	{
		if (**line == 'C')
			set_ceilfloor(*line, &game->color_ceil, game);
		else if (**line == 'F')
			set_ceilfloor(*line, &game->color_floor, game);
		else if (**line == 'N' || **line == 'S' || \
				**line == 'W' || **line == 'E')
			set_textures(*line, game);
		else if (**line != '\0')
			return ;
		free(*line);
		if (status == 0)
			terminate(game, ERR_PARSE, "There is no map in scene file");
		status = get_next_line(file_fd, line);
	}
	if (status == -1)
		terminate(game, ERR_PARSE, "Can't load scene file");
}

void	parse_map(int file_fd, char *line, t_game *game)
{
	t_list	*map;
	t_list	*line_lst;
	int		status;

	if (ft_assign_ptr((void *)&map, ft_lstnew(line)) == NULL)
		terminate(game, ERR_MEM, "Memory allocation failed (map first row)");
	game->map.size = (t_upoint){ft_strlen(line), 1};
	status = get_next_line(file_fd, &line);
	while (status >= 0 && *line != '\0')
	{
		if (ft_assign_ptr((void *)&line_lst, ft_lstnew(line)) == NULL)
			terminate(game, ERR_MEM, "Memory allocation failed (map rows)");
		ft_lstadd_front(&map, line_lst);
		game->map.size.x = ft_umax(ft_strlen(line), game->map.size.x);
		game->map.size.y++;
		status = get_next_line(file_fd, &line);
	}
	if (status == -1)
		terminate(game, ERR_PARSE, "Can't load scene file");
	free(line);
	if (status != 0)
		terminate(game, ERR_PARSE, "Empty lines in map are not allowed");
	set_map(game, map);
}

void	validate_settings(t_game *game)
{
	if (game->color_floor == -1U)
		terminate(game, ERR_PARSE, "Floor color not found. Format: F R,G,B");
	if (game->color_ceil == -1U)
		terminate(game, ERR_PARSE, "Ceil color not found. Format: C R,G,B");
	if (game->texture[WALL_N].ptr == NULL)
		terminate(game, ERR_PARSE, \
				"North wall texture doesn't set. Format: 'NO ./path.xpm'");
	if (game->texture[WALL_S].ptr == NULL)
		terminate(game, ERR_PARSE, \
				"South wall texture doesn't set. Format: 'SO ./path.xpm'");
	if (game->texture[WALL_W].ptr == NULL)
		terminate(game, ERR_PARSE, \
				"West wall texture doesn't set. Format: 'WE ./path.xpm'");
	if (game->texture[WALL_E].ptr == NULL)
		terminate(game, ERR_PARSE, \
				"East wall texture doesn't set. Format: 'EA ./path.xpm'");
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 18:27:20 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/05 17:06:46 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	parse(int args, char **av, t_game *game, bool *screenshot_only)
{
	int		file_id;
	char	*line;

	if (args == 1)
		terminate(game, ERR_ARGS, "Please specify scene filename");
	else if (args >= 4)
		terminate(game, ERR_ARGS, "Too many arguments");
	av++;
	if (ft_strlen(*av) < 5 || ft_memcmp(".cub", *av + ft_strlen(*av) - 4, 5))
		terminate(game, ERR_ARGS, "Wrong scene filename");
	if ((file_id = open(*av, O_RDONLY)) == -1)
		terminate(game, ERR_ARGS, strerror(errno));
	parse_scene(file_id, &line, game);
	validate_settings(game);
	parse_map(file_id, line, game);
	if (close(file_id) == -1)
		terminate(game, ERR_PARSE, strerror(errno));
	*screenshot_only = false;
	if (args == 3)
	{
		if (ft_strncmp("--save", *(av + 1), 7) == 0)
			*screenshot_only = true;
		else
			terminate(game, ERR_ARGS, "Invalid option");
	}
}

void	parse_scene(int file_id, char **line, t_game *game)
{
	int	status;

	game->color_floor = -1U;
	game->color_ceil = -1U;
	while ((status = get_next_line(file_id, line)) >= 0)
	{
		if (**line == 'R')
			set_resolution(*line, &game->img.size, game);
		else if (**line == 'C')
			set_colors(*line, &game->color_ceil, game);
		else if (**line == 'F')
			set_colors(*line, &game->color_floor, game);
		else if (**line == 'N' || **line == 'S' ||
				**line == 'W' || **line == 'E')
			set_textures(*line, game);
		else if (**line != '\0')
			return ;
		free(*line);
		if (status == 0)
			terminate(game, ERR_PARSE, "There is no map in scene file");
	}
	if (status == -1)
		terminate(game, ERR_PARSE, "Can't load scene file");
}

void	parse_map(int file_id, char *line, t_game *game)
{
	t_list		*map;
	t_list		*line_lst;
	int			status;
	unsigned	line_len;

	if ((map = ft_lstnew(line)) == NULL)
		terminate(game, ERR_MEM, "Memory allocation failed (map first row)");
	game->map.size = (t_upoint){ft_strlen(line), 1};
	while ((status = get_next_line(file_id, &line)) >= 0 && *line != '\0')
	{
		if ((line_lst = ft_lstnew(line)) == NULL)
			terminate(game, ERR_MEM, "Memory allocation failed (map rows)");
		ft_lstadd_front(&map, line_lst);
		if ((line_len = ft_strlen(line)) > game->map.size.x)
			game->map.size.x = line_len;
		game->map.size.y++;
	}
	if (status == -1)
		terminate(game, ERR_PARSE, "Can't load scene file");
	free(line);
	if (status != 0)
		terminate(game, ERR_PARSE, "Empty lines in map are not allowed");
	if ((game->map.grid = malloc(sizeof(char *) * game->map.size.y)) == NULL)
		terminate(game, ERR_MEM, "Memory allocation failed (map rows)");
	set_map(game, map);
}

void	validate_settings(t_game *game)
{
	if (game->color_floor == -1U)
		terminate(game, ERR_PARSE, "Floor color not found. Format: F R,G,B");
	if (game->color_ceil == -1U)
		terminate(game, ERR_PARSE, "Ceil color not found. Format: C R,G,B");
	if (game->img.size.x == 0 || game->img.size.y == 0)
		terminate(game, ERR_PARSE,
		"Resolution doesn't set. Format: 'R WIDTH HEIGHT' (max 32767x32767)");
	if (game->texture[WALL_N].ptr == NULL)
		terminate(game, ERR_PARSE,
				"North wall texture doesn't set. Format: 'NO ./path.xpm'");
	if (game->texture[WALL_S].ptr == NULL)
		terminate(game, ERR_PARSE,
				"South wall texture doesn't set. Format: 'SO ./path.xpm'");
	if (game->texture[WALL_W].ptr == NULL)
		terminate(game, ERR_PARSE,
				"West wall texture doesn't set. Format: 'WE ./path.xpm'");
	if (game->texture[WALL_E].ptr == NULL)
		terminate(game, ERR_PARSE,
				"East wall texture doesn't set. Format: 'EA ./path.xpm'");
	if (game->texture[SPRITE].ptr == NULL)
		terminate(game, ERR_PARSE,
				"Sprite texture doesn't set. Format: 'S ./path.xpm'");
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:59 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/07 11:54:37 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

bool	parse(int args, char **av, t_game *game)
{
	int		file_id;
	char	*line;

	if (ft_strncmp("--help", *av, 7) == 0)
		terminate_help(game);
	if (ft_strlen(*av) < 5 || ft_memcmp(".cub", *av + ft_strlen(*av) - 4, 5))
		terminate(game, ERR_ARGS, "Wrong scene filename");
	file_id = open(*av, O_RDONLY);
	if (file_id == -1)
		terminate(game, ERR_ARGS, strerror(errno));
	parse_scene(file_id, &line, game);
	validate_settings(game);
	parse_map(file_id, line, ft_lstnew(line), game);
	close(file_id);
	if (args == 3)
	{
		if (ft_strncmp("--save", *(av + 1), 7) == 0)
			return (true);
		else
			terminate(game, ERR_ARGS, "Invalid option");
	}
	return (false);
}

void	parse_scene(int file_id, char **line, t_game *game)
{
	int	status;

	status = get_next_line(file_id, line);
	while (status >= 0)
	{
		if (!parse_line(*line, game))
			return ;
		free(*line);
		if (status == 0)
			terminate(game, ERR_PARSE, "There is no map in scene file");
		status = get_next_line(file_id, line);
	}
	if (status == -1)
		terminate(game, ERR_PARSE, "Can't load scene file");
}

bool	parse_line(char *line, t_game *game)
{
	if (*line == 'R')
		set_resolution(line, game);
	else if (*line == 'C' || *line == 'F')
		set_ceilfloor(line, game);
	else if (*line == 'W' || *line == 'S')
		load_walls_and_sprites(line, game);
	else if (*line == 'G')
		load_weapons(line, game);
	else if (*line == 'E')
		load_enemyset(line, game);
	else if (*line == 'M' || *line == 'A')
		load_music_and_sounds(line, game);
	else if (*line == 'D')
		atoi_limited((unsigned *)&game->fade_distance, line + 1, INT_MAX);
	else if (*line == 'I' && *game->string.text == '\0')
		message_add(game, line + 2, 5, COLOR_WHITE);
	else if (*line != '#' && *line != '\0')
		return (false);
	return (true);
}

void	parse_map(int file_id, char *line, t_list *map, t_game *game)
{
	t_list		*line_lst;
	int			status;

	if (map == NULL)
		terminate(game, ERR_MEM, "Memory allocation failed (map first row)");
	game->map.size = (t_upoint){ft_strlen(line), 1};
	status = get_next_line(file_id, &line);
	while (status >= 0 && *line != '\0')
	{
		line_lst = ft_lstnew(line);
		if (line_lst == NULL)
			terminate(game, ERR_MEM, "Memory allocation failed (map rows)");
		ft_lstadd_front(&map, line_lst);
		game->map.size.x = ft_umax(ft_strlen(line), game->map.size.x);
		game->map.size.y++;
		status = get_next_line(file_id, &line);
	}
	free(line);
	if (status == -1)
		terminate(game, ERR_PARSE, "Can't load scene file");
	if (status != 0)
		terminate(game, ERR_PARSE, "Empty lines in map are not allowed");
	set_map(game, map);
}

void	validate_settings(t_game *game)
{
	uint32_t	i;

	if (game->color_floor == -1U && game->texture_floor.ptr == NULL)
		terminate(game, ERR_PARSE, "Missing Floor color/texture");
	if (game->color_ceil == -1U && game->texture_ceil.ptr == NULL)
		terminate(game, ERR_PARSE, "Missing Ceil color/texture");
	if (game->resolution.x == 0 || game->resolution.y == 0)
		terminate(game, ERR_PARSE, \
						"Resolution doesn't set. Format: 'R WIDTH HEIGHT'");
	i = 0;
	while (i < sizeof(game->texture) / sizeof(*game->texture) / 2)
		if (game->texture[i++].ptr == NULL)
			terminate(game, ERR_PARSE, "Missing wall texture");
	i = 0;
	while (i < sizeof(game->sprite) / sizeof(*game->sprite))
		if (game->sprite[i++].ptr == NULL)
			terminate(game, ERR_PARSE, "Missing sprite");
	i = 0;
	while (i < sizeof(game->p.weapon_img) / sizeof(*game->p.weapon_img))
		if (game->p.weapon_img[i++][0].ptr == NULL)
			terminate(game, ERR_PARSE, "Missing weapon texture");
}

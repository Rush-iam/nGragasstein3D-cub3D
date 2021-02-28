/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 18:27:20 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/28 18:55:54 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	parse(int args, char **av, t_game *game)
{
	int		file_id;
	char	*line;

	if (args == 1)
		terminate(ERROR_ARGS, "Please specify scene filename");
	else if (args >= 4)
		terminate(ERROR_ARGS, "Too many arguments");
	av++;
	if (ft_strlen(*av) < 5 || ft_memcmp(".cub", *av + ft_strlen(*av) - 4, 5))
		terminate(ERROR_ARGS, "Wrong scene filename");
	if ((file_id = open(*av, O_RDONLY)) == -1)
		terminate(ERROR_ARGS, strerror(errno));
	parse_scene(file_id, &line, game);
	parse_map(file_id, line, game);
	if (close(file_id) == -1)
		terminate(ERROR_PARSE, strerror(errno));
	if (args == 3)
	{
		if (ft_strncmp("--save", *++av, 7) == 0)
			write_screenshot();
		else
			terminate(ERROR_ARGS, "Invalid option");
	}
}

void	parse_scene(int file_id, char **line, t_game *game)
{
	int	status;

	game->color_floor = -1;
	game->color_ceil = -1;
	while ((status = get_next_line(file_id, line)) >= 0)
	{
		if (**line == 'R')
			set_resolution(*line, &game->img.size);
		else if (**line == 'C')
			set_colors(*line, &game->color_ceil);
		else if (**line == 'F')
			set_colors(*line, &game->color_floor);
		else if (**line == 'N' || **line == 'S' ||
				**line == 'W' || **line == 'E')
			set_textures(*line, game);
		else if (**line != '\0')
			return ;
		free(*line);
		if (status == 0)
			terminate(ERROR_PARSE, "There is no map in scene file");
	}
	if (status == -1)
		terminate(ERROR_PARSE, "Something went wrong while reading scene file");
}

void	set_resolution(const char *res_string, t_upoint *res)
{
	if (res->x)
		terminate(ERROR_PARSE, "Duplicated R(esolution) setting in scene file");
	res_string++;
	res_string = atoi_limited(&res->x, res_string, USHRT_MAX);
	if (res_string == NULL)
		terminate(ERROR_PARSE, "R(esolution) X setting is too large (>65535)");
	res_string = atoi_limited(&res->y, res_string, USHRT_MAX);
	if (res_string == NULL)
		terminate(ERROR_PARSE, "R(esolution) Y setting is too large (>65535)");
	if (*res_string != '\0' || res->x == 0 || res->y == 0)
		terminate(ERROR_PARSE, "Wrong R(esolution) setting");
}

void	set_colors(const char *color_string, unsigned *target)
{
	unsigned	r;
	unsigned	g;
	unsigned	b;

	r = 0;
	g = 0;
	b = 0;
	if (*target != (unsigned)-1)
		terminate(ERROR_PARSE, "Duplicated F or C color setting in scene file");
	color_string++;
	color_string = atoi_limited(&r, color_string, UCHAR_MAX);
	if (color_string == NULL)
		terminate(ERROR_PARSE, "F or C color R(ed) value is too large (>255)");
	if (*color_string++ != ',')
		terminate(ERROR_PARSE, "F or C color format: 'F R,G,B' or 'C R,G,B'");
	color_string = atoi_limited(&g, color_string, UCHAR_MAX);
	if (color_string == NULL)
		terminate(ERROR_PARSE, "F or C color R(ed) value is too large (>255)");
	if (*color_string++ != ',')
		terminate(ERROR_PARSE, "F or C color format: 'F R,G,B' or 'C R,G,B'");
	color_string = atoi_limited(&b, color_string, UCHAR_MAX);
	if (color_string == NULL)
		terminate(ERROR_PARSE, "F or C color R(ed) value is too large (>255)");
	*target = (r << 16) | (g << 8) | b;
}

void	set_textures(char *string, t_game *game)
{
	int	id;
	int null;

	if (string[0] == 'S' && string[1] == ' ')
		id = SPRITE;
	else if (string[0] == 'N' && string[1] == 'O')
		id = WALL_N;
	else if (string[0] == 'S' && string[1] == 'O')
		id = WALL_S;
	else if (string[0] == 'W' && string[1] == 'E')
		id = WALL_W;
	else if (string[0] == 'E' && string[1] == 'A')
		id = WALL_E;
	string += 2;
	while (*string == ' ')
		string++;
	if (game->texture[id].ptr != NULL)
		terminate(ERROR_PARSE, "Duplicated texture setting in scene file");
	if (!(game->texture[id].ptr = mlx_xpm_file_to_image(game->mlx, string,
		(int *)&game->texture[id].size.x, (int *)&game->texture[id].size.y)))
		terminate(ERROR_PARSE, "Something went wrong while loading texture");
	game->texture[id].data = (unsigned *)mlx_get_data_addr(
								game->texture[id].ptr, &null, &null, &null);
	game->texture[id].aspect = game->texture[id].size.x /
								game->texture[id].size.y;
}

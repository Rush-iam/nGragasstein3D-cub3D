/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_set.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 18:43:41 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/04 15:57:36 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	set_resolution(const char *res_string, t_upoint *res, t_game *game)
{
	if (res->x)
		terminate(game, ERR_PARSE, "Duplicated Resolution setting");
	res_string++;
	res_string = atoi_limited(&res->x, res_string, UINT_MAX);
	if (res_string == NULL)
		terminate(game, ERR_PARSE, "Resolution X setting is wrong");
	res_string = atoi_limited(&res->y, res_string, UINT_MAX);
	if (res_string == NULL)
		terminate(game, ERR_PARSE, "Resolution Y setting is wrong");
	else if (*res_string != '\0' || res->x == 0 || res->y == 0)
		terminate(game, ERR_PARSE, "Wrong Resolution setting");
}

void	set_ceilfloor(const char *color_string, unsigned *target, t_game *game)
{
	unsigned	r;
	unsigned	g;
	unsigned	b;

	if (*target != (unsigned)-1)
		terminate(game, ERR_PARSE, "Duplicated F or C color setting");
	color_string++;
	color_string = atoi_limited(&r, color_string, UCHAR_MAX);
	if (color_string == NULL)
		terminate(game, ERR_PARSE, "F/C color Red is wrong (range: 0-255)");
	else if (*color_string++ != ',')
		terminate(game, ERR_PARSE, "F/C color format: 'F R,G,B'/'C R,G,B'");
	color_string = atoi_limited(&g, color_string, UCHAR_MAX);
	if (color_string == NULL)
		terminate(game, ERR_PARSE, "F/C color Green is wrong (range: 0-255)");
	else if (*color_string++ != ',')
		terminate(game, ERR_PARSE, "F/C color format: 'F R,G,B'/'C R,G,B'");
	color_string = atoi_limited(&b, color_string, UCHAR_MAX);
	if (color_string == NULL)
		terminate(game, ERR_PARSE, "F/C color Blue is wrong (range: 0-255)");
	else if (*color_string != '\0')
		terminate(game, ERR_PARSE, "F/C color line redundant symbols");
	*target = (r << 16) | (g << 8) | b;
}

void	set_textures(char *string, t_game *game)
{
	int	id;

	id = -1;
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
	else
		terminate(game, ERR_PARSE, "Wrong texture setting. Valid: NO/SO/WE/EA");
	if (game->texture[id].ptr != NULL)
		terminate(game, ERR_PARSE, "Duplicated texture setting");
	string += 2;
	while (*string == ' ')
		string++;
	set_textures_import(string, &game->texture[id], game);
}

void	set_textures_import(char *string, t_img *texture, t_game *game)
{
	int				null;
	const size_t	str_len = ft_strlen(string);

	if (str_len < 5)
		terminate(game, ERR_ARGS, "Can't identify texture format (.xpm/.png)");
	if (ft_memcmp(".xpm", string + str_len - 4, 5) == 0)
	{
		if (!(texture->ptr = mlx_xpm_file_to_image(game->mlx, string,
							(int *)&texture->size.x, (int *)&texture->size.y)))
			terminate(game, ERR_PARSE, "Can't load texture file");
	}
	else if (ft_memcmp(".png", string + str_len - 4, 5) == 0)
	{
		if (!(texture->ptr = mlx_png_file_to_image(game->mlx, string,
							(int *)&texture->size.x, (int *)&texture->size.y)))
			terminate(game, ERR_PARSE, "Can't load texture file");
	}
	else
		terminate(game, ERR_ARGS, "Can't identify texture format (.xpm/.png)");
	texture->data = (unsigned *)mlx_get_data_addr(texture->ptr, &null, &null,
																		&null);
	texture->aspect = texture->size.x / texture->size.y;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_set_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:55 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/13 22:23:00 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

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
	if (*res_string != '\0' || res->x == 0 || res->y == 0)
		terminate(game, ERR_PARSE, "Wrong Resolution setting");
}

void	set_colors(const char *color_string, unsigned *target, t_game *game)
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
	if (*color_string++ != ',')
		terminate(game, ERR_PARSE, "F/C color format: 'F R,G,B'/'C R,G,B'");
	color_string = atoi_limited(&g, color_string, UCHAR_MAX);
	if (color_string == NULL)
		terminate(game, ERR_PARSE, "F/C color Green is wrong (range: 0-255)");
	if (*color_string++ != ',')
		terminate(game, ERR_PARSE, "F/C color format: 'F R,G,B'/'C R,G,B'");
	color_string = atoi_limited(&b, color_string, UCHAR_MAX);
	if (color_string == NULL)
		terminate(game, ERR_PARSE, "F/C color Blue is wrong (range: 0-255)");
	if (*color_string != '\0')
		terminate(game, ERR_PARSE, "F/C color line redundant symbols");
	*target = (r << 16) | (g << 8) | b;
}


void	set_weapons(char *string, t_game *game)
{
	unsigned	id;
	int			i;
	char		*path;

	if ((string = atoi_limited(&id, string + 1, 100)) == NULL)
		terminate(game, ERR_PARSE, "Weapon ID is wrong (Gx)");
	if (id >= sizeof(game->p.weapon_img) / sizeof(*game->p.weapon_img))
		terminate(game, ERR_PARSE, "Weapon ID out of array range");
	if (game->p.weapon_img[id][0].ptr != NULL)
		terminate(game, ERR_PARSE, "Duplicated weapon setting");
	i = 0;
	while (i < 4)
	{
		if ((path = ft_strjoin(string,
						(char []){'0' + i, '.', 'p', 'n', 'g', '\0'})) == NULL)
			terminate(game, ERR_PARSE, strerror(errno));
		load_texture_file(path, &game->p.weapon_img[id][i],
										"Can't load weapon texture file", game);
		free(path);
		i++;
	}
}

void	set_textures(char *string, t_game *game)
{
	unsigned	id;

	if (*string == 'W')
	{
		if ((string = atoi_limited(&id, string + 1, 100)) == NULL)
			terminate(game, ERR_PARSE, "Wall ID is wrong");
		if (id >= sizeof(game->texture) / sizeof(*game->texture))
			terminate(game, ERR_PARSE, "Texture ID out of array range");
		if (game->texture[id].ptr != NULL)
			terminate(game, ERR_PARSE, "Duplicated texture setting");
		load_texture_file(string, &game->texture[id],
										"Can't load wall texture file", game);
	}
	else if (*string == 'S')
	{
		if ((string = atoi_limited(&id, string + 1, 100)) == NULL)
			terminate(game, ERR_PARSE, "Sprite ID is wrong");
		if (id >= sizeof(game->sprite) / sizeof(*game->sprite))
			terminate(game, ERR_PARSE, "Sprite ID out of array range");
		if (game->sprite[id].ptr != NULL)
			terminate(game, ERR_PARSE, "Duplicated sprite setting");
		load_texture_file(string, &game->sprite[id],
										"Can't load sprite file", game);
	}
}

void	load_spriteset(t_img dst[], int count, char *path, t_game *game)
{
	int		i;
	char	*path2;

	i = 0;
	while (i < count)
	{
		if ((path2 = ft_strjoin(path,
							(char []){'0' + i, '.', 'p', 'n', 'g', 0})) == NULL)
			terminate(game, ERR_PARSE, strerror(errno));
		load_texture_file(path2, &dst[i], "Can't load enemy sprite file", game);
		free(path2);
		i++;
	}
	free(path);
}

void	load_texture_file(char *path, t_img *dst_img, char *err, t_game *game)
{
	int				null;
	const size_t	str_len = ft_strlen(path);

	if (str_len < 5)
		terminate(game, ERR_ARGS, "Can't identify texture format (.xpm/.png)");
	if (ft_memcmp(".xpm", path + str_len - 4, 5) == 0)
	{
		if (!(dst_img->ptr = mlx_xpm_file_to_image(game->mlx, path,
							(int *)&dst_img->size.x, (int *)&dst_img->size.y)))
			terminate(game, ERR_PARSE, err);
	}
	else if (ft_memcmp(".png", path + str_len - 4, 5) == 0)
	{
		if (!(dst_img->ptr = mlx_png_file_to_image(game->mlx, path,
							(int *)&dst_img->size.x, (int *)&dst_img->size.y)))
			terminate(game, ERR_PARSE, err);
	}
	else
		terminate(game, ERR_ARGS, "Can't identify texture format (.xpm/.png)");
	dst_img->data = (unsigned *)mlx_get_data_addr(dst_img->ptr, &null, &null,
												  &null);
	dst_img->aspect = dst_img->size.x / dst_img->size.y;
}

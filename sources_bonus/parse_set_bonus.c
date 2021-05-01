/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_set_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:55 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/30 23:21:27 by ngragas          ###   ########.fr       */
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
	else if (*res_string != '\0' || res->x == 0 || res->y == 0)
		terminate(game, ERR_PARSE, "Wrong Resolution setting");
}

void	set_ceilfloor(char *string, t_game *game)
{
	if (*string == 'C' && *(string + 1) != 'T' && !game->texture_ceil.ptr)
		set_ceilfloor_color(string + 1, &game->color_ceil, game);
	else if (*string == 'F' && *(string + 1) != 'T' && !game->texture_floor.ptr)
		set_ceilfloor_color(string + 1, &game->color_floor, game);
	else if (*string == 'C' && *(string + 1) == 'T')
	{
		string += 2;
		if (game->texture_ceil.ptr == NULL && game->color_ceil == -1U)
			load_image_file(game, skip_spaces(&string), &game->texture_ceil,
			"Failed to load ceil texture file");
		else
			terminate(game, ERR_PARSE, "Duplicated Ceil setting");
	}
	else if (*string == 'F' && *(string + 1) == 'T')
	{
		string += 2;
		if (game->texture_floor.ptr == NULL && game->color_floor == -1U)
			load_image_file(game, skip_spaces(&string), &game->texture_floor,
			"Failed to load floor texture file");
		else
			terminate(game, ERR_PARSE, "Duplicated Floor setting");
	}
	else
		terminate(game, ERR_PARSE, "Duplicated F or C color setting");
}

void	set_ceilfloor_color(char *string, uint32_t *target, t_game *game)
{
	uint32_t	r;
	uint32_t	g;
	uint32_t	b;

	if (*target != -1U)
		terminate(game, ERR_PARSE, "Duplicated F or C color setting");
	string = atoi_limited(&r, string, UCHAR_MAX);
	if (string == NULL)
		terminate(game, ERR_PARSE, "F/C color Red is wrong (range: 0-255)");
	else if (*string++ != ',')
		terminate(game, ERR_PARSE, "F/C color format: 'F R,G,B'/'C R,G,B'");
	string = atoi_limited(&g, string, UCHAR_MAX);
	if (string == NULL)
		terminate(game, ERR_PARSE, "F/C color Green is wrong (range: 0-255)");
	else if (*string++ != ',')
		terminate(game, ERR_PARSE, "F/C color format: 'F R,G,B'/'C R,G,B'");
	string = atoi_limited(&b, string, UCHAR_MAX);
	if (string == NULL)
		terminate(game, ERR_PARSE, "F/C color Blue is wrong (range: 0-255)");
	else if (*string != '\0')
		terminate(game, ERR_PARSE, "F/C color line redundant symbols");
	*target = (r << 16) | (g << 8) | b;
}

void	set_weapons(char *string, t_game *game)
{
	uint32_t	id;
	uint32_t	i;
	char		*path;

	string++;
	if (!ft_isdigit(*string))
		terminate(game, ERR_PARSE, "Weapon ID is wrong (Gx)");
	string = atoi_limited(&id, string, 100);
	if (string == NULL)
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
		load_image_file(game, path, &game->p.weapon_img[id][i],
						"Can't load weapon texture file");
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
		if (id >= sizeof(game->texture) / sizeof(*game->texture) / 2)
			terminate(game, ERR_PARSE, "Texture ID out of array range");
		if (game->texture[id].ptr != NULL)
			terminate(game, ERR_PARSE, "Duplicated texture setting");
		load_image_file(game, string, &game->texture[id],
						"Can't load wall texture file");
	}
	else if (*string == 'S')
	{
		if ((string = atoi_limited(&id, string + 1, 100)) == NULL)
			terminate(game, ERR_PARSE, "Sprite ID is wrong");
		if (id >= sizeof(game->sprite) / sizeof(*game->sprite))
			terminate(game, ERR_PARSE, "Sprite ID out of array range");
		if (game->sprite[id].ptr != NULL)
			terminate(game, ERR_PARSE, "Duplicated sprite setting");
		load_image_file(game, string, &game->sprite[id],
						"Can't load sprite file");
	}
}

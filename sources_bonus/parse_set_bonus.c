/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_set_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:55 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 16:05:26 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	set_resolution(const char *res_string, t_game *game)
{
	if (game->resolution.x)
		terminate(game, ERR_PARSE, "Duplicated Resolution setting");
	res_string++;
	if (*res_string != ' ')
		terminate(game, ERR_PARSE, "Add space after Resolution identifier");
	res_string = atoi_limited(&game->resolution.x, res_string, UINT_MAX);
	if (res_string == NULL)
		terminate(game, ERR_PARSE, "Resolution X setting is wrong");
	res_string = atoi_limited(&game->resolution.y, res_string, UINT_MAX);
	if (res_string == NULL)
		terminate(game, ERR_PARSE, "Resolution Y setting is wrong");
	else if (*res_string != '\0' || !game->resolution.x || !game->resolution.y)
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
			load_image_file(game, skip_spaces(&string), &game->texture_ceil, \
			"Failed to load ceil texture file");
		else
			terminate(game, ERR_PARSE, "Duplicated Ceil setting");
	}
	else if (*string == 'F' && *(string + 1) == 'T')
	{
		string += 2;
		if (game->texture_floor.ptr == NULL && game->color_floor == -1U)
			load_image_file(game, skip_spaces(&string), &game->texture_floor, \
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
	if (*string != ' ')
		terminate(game, ERR_PARSE, "Add space after F/C identifier");
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

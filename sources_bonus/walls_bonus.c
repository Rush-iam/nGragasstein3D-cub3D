/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:35 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/11 16:27:10 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_walls(t_game *game)
{
	unsigned	ray;
	float		fade;
	unsigned	texture_id;

	ray = 0;
	while (ray < game->img.size.x)
	{
		fade = 8.f / game->column[ray]->distance;
		if (fade > 1)
			fade = 1;
		if (game->column[ray]->dir == 'W' || game->column[ray]->dir == 'E')
			fade /= 2;
		texture_id = game->map.grid[(int)game->column[ray]->cell.y]
									[(int)game->column[ray]->cell.x] - '0';
		draw_wall_scaled(game, &game->texture[texture_id], ray, fade);
		ray++;
	}
}

void	draw_wall_scaled(t_game *game, t_img *src, unsigned x, float fade)
{
	const double	step = (double)src->size.y / game->column[x]->height;
	const unsigned	src_x = game->column[x]->texture_pos * src->size.x;
	double			src_y;
	int				y;
	int				max_height;

	if (game->column[x]->height > game->img.size.y)
	{
		src_y = step * (game->column[x]->height - game->img.size.y) / 2;
		y = -1;
		max_height = game->img.size.y;
	}
	else
	{
		src_y = 0;
		y = -1 + (game->img.size.y - game->column[x]->height) / 2;
		max_height = y + game->column[x]->height;
	}
	while (++y < max_height)
	{
		game->img.data[y * game->img.size.x + x] = (fade == 1) ?
			src->data[(unsigned)src_y * src->size.x + src_x] :
			pixel_fade(src->data[(unsigned)src_y * src->size.x + src_x], fade);
		src_y += step;
	}
}

void	draw_wall_solid(t_game *game, unsigned x, float fade)
{
	draw_line(&game->img,
		(t_point){x, (game->img.size.y - game->column[x]->height) / 2},
		(t_point){x, (game->img.size.y + game->column[x]->height) / 2},
		((int)(0x60 * fade) << 16) + ((int)(0x80 * fade) << 8) +
		(int)(0x80 * fade));
}

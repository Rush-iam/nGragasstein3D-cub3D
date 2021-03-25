/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_walls_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 23:32:38 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/25 23:01:12 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_walls(t_game *game)
{
	unsigned	ray;
	float		fade;

	ray = 0;
	while (ray < game->img.size.x)
	{
		if (game->column[ray].texture_id < sizeof(game->texture) /
												sizeof(*game->texture))
		{
			fade = fminf(8.0f / (float)game->column[ray].distance, 1.0f);
			draw_wall_scaled(game, game->texture[game->column[ray].texture_id],
																	ray, fade);
//			draw_wall_solid(game, ray, fade);
		}
		ray++;
	}
}

void	draw_wall_scaled(t_game *g, t_img src, unsigned x, float fade)
{
	const float		step = (float)src.size.y / g->column[x].height;
	const unsigned	src_x = g->column[x].texture_pos * (src.size.x - 1);
	float			src_y;
	unsigned		y;
	const unsigned	half = ft_umin(g->win_center.y, g->column[x].height / 2);

	g->img.data[g->win_center.y * g->img.size.x + x] = (fade == 1) ?
	src.data[(int)(src.size.y / 2.f) * src.size.x + src_x] :
	pixel_fade(src.data[(int)(src.size.y / 2.f) * src.size.x + src_x], fade);
	y = 1;
	src_y = step;
	while (y < half)
	{
		g->img.data[(g->win_center.y - y) * g->img.size.x + x] = (fade == 1) ?
			src.data[(int)(src.size.y / 2.f - src_y) * src.size.x + src_x] :
	 		pixel_fade(src.data[(int)(src.size.y / 2.f - src_y) * src.size.x +
	 			src_x], fade);
		g->img.data[(g->win_center.y + y) * g->img.size.x + x] = (fade == 1) ?
			src.data[(int)(src.size.y / 2.f + src_y) * src.size.x + src_x] :
			pixel_fade(src.data[(int)(src.size.y / 2.f + src_y) * src.size.x +
				src_x], fade);
		src_y += step;
		y++;
	}
}

void	draw_wall_solid(t_game *game, unsigned x, float fade)
{
	unsigned	start_y;
	unsigned	end_y;

	if (game->column[x].height >= game->img.size.y)
	{
		start_y = 0;
		end_y = game->img.size.y;
	}
	else
	{
		start_y = ((int)game->img.size.y - (int)game->column[x].height) / 2;
		end_y = start_y + game->column[x].height;

	}
	(void)fade;
	while (start_y < end_y)
//		game->img.data[start_y++ + x * game->img.size.x] = 0x608080;
//		game->img.data[start_y++ * game->img.size.x + x] = 0x608080;
		game->img.data[start_y++ * game->img.size.x + x] =
				((int)(0x60 * fade) << 16) + ((int)(0x80 * fade) << 8) +
				(int)(0x80 * fade);
}

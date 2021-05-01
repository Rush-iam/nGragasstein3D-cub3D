/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_walls_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 23:32:38 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 15:24:45 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_walls(t_game *g)
{
	uint32_t	ray;

	ray = 0;
	g->height_step_up = 1.0f / (0.5f - g->z_level);
	g->height_step_down = 1.0f / (0.5f + g->z_level);
	while (ray < g->img.size.x)
	{
		if ((uint32_t)g->column[ray].cell.x < g->map.size.x && \
			(uint32_t)g->column[ray].cell.y < g->map.size.y)
		{
			draw_texture_set(g, &g->column[ray]);
			__sincosf(g->p.angle + g->angles[ray], \
						&g->ray_vector.y, &g->ray_vector.x);
			g->ray_vector.x /= cosf(g->angles[ray]);
			g->ray_vector.y /= cosf(g->angles[ray]);
			g->column[ray].height = \
					(int)(g->col_scale / g->column[ray].distance) & ~1;
			draw_wall_scaled(g, g->texture[g->column[ray].texture_id], \
					(t_point){ray, 0}, g->z_level * g->column[ray].height);
		}
		ray++;
	}
}

void	draw_wall_scaled(t_game *g, t_img src, t_point cur, int z_offset)
{
	const float		fade = g->fade_distance / g->column[cur.x].distance;
	const float		step = (float)src.size.y / g->column[cur.x].height;
	const int		src_x = g->column[cur.x].texture_pos * src.size.x;
	float			src_y;
	const t_point	minmax_y = {\
	ft_max(0, g->horizon + z_offset - g->column[cur.x].height / 2), \
	ft_min(g->img.size.y, g->horizon + z_offset + g->column[cur.x].height / 2)};

	src_y = fmaxf(0.0f, \
				step * (g->column[cur.x].height / 2 - g->horizon - z_offset));
	cur.y = minmax_y.x;
	if (g->texture_ceil.ptr && cur.y)
		draw_ceil_textured(g, cur);
	while (cur.y < minmax_y.y)
	{
		if (fade > 1.0f)
			g->img.data[g->img.size.x * cur.y++ + cur.x] = \
				src.data[(int)src_y * src.size.x + src_x];
		else
			g->img.data[g->img.size.x * cur.y++ + cur.x] = \
				pixel_fade(src.data[(int)src_y * src.size.x + src_x], fade);
		src_y += step;
	}
	if (g->texture_floor.ptr && cur.y != (int)g->img.size.y)
		draw_floor_textured(g, (t_point){cur.x, minmax_y.y});
}

void	draw_wall_solid(t_game *game, uint32_t x, float fade)
{
	uint32_t	start_y;
	uint32_t	end_y;

	if (game->column[x].height >= (int)game->img.size.y)
	{
		start_y = 0;
		end_y = game->img.size.y;
	}
	else
	{
		start_y = ((int)game->img.size.y - game->column[x].height) / 2;
		end_y = start_y + game->column[x].height;
	}
	while (start_y < end_y)
	{
		game->img.data[game->img.size.x * start_y + x] = \
			((int)(0x60 * fade) << 16) + ((int)(0x80 * fade) << 8) + \
			(int)(0x80 * fade);
		start_y++;
	}
}

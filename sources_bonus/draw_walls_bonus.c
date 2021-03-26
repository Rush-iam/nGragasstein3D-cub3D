/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_walls_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 23:32:38 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/26 17:59:51 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_wall_texture_set(t_game *game, struct s_column *col, t_point cell)
{
	const char	chr = game->map.grid[cell.y][cell.x];

	if ((chr == '>' && (col->dir == 'W' || col->dir == 'E')) ||
		(chr == 'v' && (col->dir == 'N' || col->dir == 'S')))
	{
		col->texture_id = TEXTURE_DOOR;
		col->texture_pos =
			fmaxf(0.0f, col->texture_pos - door_find(game, cell)->part_opened);
	}
	else if ((chr == 'v' && (col->dir == 'W' || col->dir == 'E')) ||
			 (chr == '>' && (col->dir == 'N' || col->dir == 'S')))
		col->texture_id = TEXTURE_DOOR_W;
	else if ((col->dir == 'N' && game->map.grid[cell.y + 1][cell.x] == '>') ||
			 (col->dir == 'S' && game->map.grid[cell.y - 1][cell.x] == '>') ||
			 (col->dir == 'W' && game->map.grid[cell.y][cell.x + 1] == 'v') ||
			 (col->dir == 'E' && game->map.grid[cell.y][cell.x - 1] == 'v'))
		col->texture_id = TEXTURE_DOOR_W;
	else
		col->texture_id = chr - '0';
	if (((col->dir == 'W' || col->dir == 'E') && col->texture_id !=
			TEXTURE_DOOR_W) || (col->texture_id == TEXTURE_DOOR_W &&
								(col->dir == 'N' || col->dir == 'S')))
		col->texture_id += sizeof(game->texture) / sizeof(*game->texture) / 2;
	if (chr != '>' && chr != 'v' && (col->dir == 'S' || col->dir == 'W'))
		col->texture_pos = 1.0f - col->texture_pos;
}

void	draw_walls(t_game *g)
{
	unsigned	ray;
	float		fade;

	ray = 0;
	while (ray < g->img.size.x)
	{
		if (g->column[ray].texture_id < sizeof(g->texture) /
										sizeof(*g->texture))
		{
			fade = fminf(1.0f, 8.0f / (float)g->column[ray].distance);
			if (fade == 1.0f)
				draw_wall_scaled(g, g->texture[g->column[ray].texture_id], ray);
			else
				draw_wall_scaled_f(g, g->texture[g->column[ray].texture_id],
																	ray, fade);
//			draw_wall_solid(game, ray, fade);
		}
		ray++;
	}
}

void	draw_wall_scaled(t_game *g, t_img src, unsigned x)
{
	const float		step = (float)src.size.y / g->column[x].height;
	const unsigned	src_x = g->column[x].texture_pos * src.size.x;
	float			src_y;
	unsigned		y;
	const unsigned	half = ft_umin(g->win_center.y, g->column[x].height / 2);

	g->img.data[g->win_center.y * g->img.size.x + x] =
			src.data[(int)(src.size.y / 2.f) * src.size.x + src_x];
	y = 1;
	src_y = step;
	while (y < half)
	{
		g->img.data[(g->win_center.y - y) * g->img.size.x + x] =
				src.data[(int)(src.size.y / 2.f - src_y) * src.size.x + src_x];
		g->img.data[(g->win_center.y + y) * g->img.size.x + x] =
				src.data[(int)(src.size.y / 2.f + src_y) * src.size.x + src_x];
		src_y += step;
		y++;
	}
}

void	draw_wall_scaled_f(t_game *g, t_img src, unsigned x, float fade)
{
	const float		step = (float)src.size.y / g->column[x].height;
	const unsigned	src_x = g->column[x].texture_pos * src.size.x;
	float			src_y;
	unsigned		y;
	const unsigned	half = ft_umin(g->win_center.y, g->column[x].height / 2);

	g->img.data[g->win_center.y * g->img.size.x + x] =
	pixel_fade(src.data[(int)(src.size.y / 2.f) * src.size.x + src_x], fade);
	y = 1;
	src_y = step;
	while (y < half)
	{
		g->img.data[(g->win_center.y - y) * g->img.size.x + x] = pixel_fade(
		src.data[(int)(src.size.y / 2.f - src_y) * src.size.x + src_x], fade);
		g->img.data[(g->win_center.y + y) * g->img.size.x + x] = pixel_fade(
		src.data[(int)(src.size.y / 2.f + src_y) * src.size.x + src_x], fade);
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

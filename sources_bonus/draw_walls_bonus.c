/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_walls_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 23:32:38 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/28 22:28:13 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_door_texture_set(t_game *game, struct s_column *col, char chr)
{
	if (chr == CHAR_DOOR_1[0] || chr == CHAR_DOOR_1[1])
		col->texture_id = TEXTURE_DOOR_1;
	else if (chr == CHAR_DOOR_2[0] || chr == CHAR_DOOR_2[1])
		col->texture_id = TEXTURE_DOOR_2;
	if (col->dir == 'N' || col->dir =='S')
		col->texture_pos = col->pos.x - (int)col->pos.x;
	else if (col->dir == 'W' || col->dir =='E')
		col->texture_pos = col->pos.y - (int)col->pos.y;
	col->texture_pos -= door_find(game, col->cell)->part_opened;
//	col->texture_pos =
//		fmaxf(0.0f, col->texture_pos - door_find(game, col->cell)->part_opened);
	if (col->dir == 'W' || col->dir == 'E')
		col->texture_id += sizeof(game->texture) / sizeof(*game->texture) / 2;
}

void	draw_wall_texture_set(t_game *g, struct s_column *col, t_point pt)
{
	const int	textures = sizeof(g->texture) / sizeof(*g->texture) / 2;

	if (col->dir == 'N' && g->map.grid[pt.y + 1][pt.x] == CHAR_DOORS_V[0])
		col->texture_id = TEXTURE_DOOR_1_W + textures;
	else if (col->dir == 'N' && g->map.grid[pt.y + 1][pt.x] == CHAR_DOORS_V[1])
		col->texture_id = TEXTURE_DOOR_2_W + textures;
	else if (col->dir == 'S' && g->map.grid[pt.y - 1][pt.x] == CHAR_DOORS_V[0])
		col->texture_id = TEXTURE_DOOR_1_W + textures;
	else if (col->dir == 'S' && g->map.grid[pt.y - 1][pt.x] == CHAR_DOORS_V[1])
		col->texture_id = TEXTURE_DOOR_2_W + textures;
	else if (col->dir == 'W' && g->map.grid[pt.y][pt.x + 1] == CHAR_DOORS_H[0])
		col->texture_id = TEXTURE_DOOR_1_W;
	else if (col->dir == 'W' && g->map.grid[pt.y][pt.x + 1] == CHAR_DOORS_H[1])
		col->texture_id = TEXTURE_DOOR_2_W;
	else if (col->dir == 'E' && g->map.grid[pt.y][pt.x - 1] == CHAR_DOORS_H[0])
		col->texture_id = TEXTURE_DOOR_1_W;
	else if (col->dir == 'E' && g->map.grid[pt.y][pt.x - 1] == CHAR_DOORS_H[1])
		col->texture_id = TEXTURE_DOOR_2_W;
	else if (col->dir == 'W' || col->dir == 'E')
		col->texture_id = g->map.grid[pt.y][pt.x] - '0' + textures;
	else
		col->texture_id = g->map.grid[pt.y][pt.x] - '0';
	if (col->dir == 'N')
		col->texture_pos = col->pos.x - (int)col->pos.x;
	else if (col->dir == 'E')
		col->texture_pos = col->pos.y - (int)col->pos.y;
	else if (col->dir == 'S')
		col->texture_pos = 1.0f - (col->pos.x - (int)col->pos.x);
	else if (col->dir == 'W')
		col->texture_pos = 1.0f - (col->pos.y - (int)col->pos.y);
}

void	draw_walls(t_game *g)
{
	unsigned	ray;
	char		chr;

	ray = 0;
	while (ray < g->img.size.x)
	{
		if ((unsigned)g->column[ray].cell.x < g->map.size.x &&
			(unsigned)g->column[ray].cell.y < g->map.size.y)
		{
			chr = g->map.grid[g->column[ray].cell.y][g->column[ray].cell.x];
			if (ft_isdigit(chr))
				draw_wall_texture_set(g, &g->column[ray], g->column[ray].cell);
			else if (ft_memchr(CHAR_DOORS, chr, sizeof(CHAR_DOORS)))
				draw_door_texture_set(g, &g->column[ray], chr);
			g->column[ray].height = g->col_scale / g->column[ray].distance;
			g->column[ray].height &= ~1;
			draw_wall_scaled(g, g->texture[g->column[ray].texture_id],
						(t_point){ray, 0}, g->z_level * g->column[ray].height);
//			draw_wall_solid(game, ray, fade);
		}
		ray++;
	}
}

void	draw_wall_scaled(t_game *g, t_img src, t_point cur, int z_offset)
{
	const float	fade = g->fade_distance / g->column[cur.x].distance;
	const float	step = (float)src.size.y / g->column[cur.x].height;
	const int	src_x = g->column[cur.x].texture_pos * src.size.x;
	float		src_y;
	const int	max_y = ft_min(g->img.size.y,
						g->horizon + z_offset + g->column[cur.x].height / 2);

	cur.y = ft_max(0, g->horizon + z_offset - g->column[cur.x].height / 2);
	src_y = fmaxf(0.0f,
				step * (g->column[cur.x].height / 2 - g->horizon - z_offset));
	if (fade < 1.0f)
		while (cur.y < max_y)
		{
			g->img.data[cur.y++ * g->img.size.x + cur.x] =
				pixel_fade(src.data[(int)src_y * src.size.x + src_x], fade);
			src_y += step;
		}
	else
		while (cur.y < max_y)
		{
			g->img.data[cur.y++ * g->img.size.x + cur.x] =
				src.data[(int)src_y * src.size.x + src_x];
			src_y += step;
		}
}

void	draw_wall_solid(t_game *game, unsigned x, float fade)
{
	unsigned	start_y;
	unsigned	end_y;

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
	(void)fade;
	while (start_y < end_y)
//		game->img.data[start_y++ + x * game->img.size.x] = 0x608080;
//		game->img.data[start_y++ * game->img.size.x + x] = 0x608080;
		game->img.data[start_y++ * game->img.size.x + x] =
				((int)(0x60 * fade) << 16) + ((int)(0x80 * fade) << 8) +
				(int)(0x80 * fade);
}

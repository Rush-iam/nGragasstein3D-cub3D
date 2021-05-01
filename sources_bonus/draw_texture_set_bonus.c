/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_texture_set_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 15:19:43 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 15:20:10 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_texture_set(t_game *g, struct s_column *col)
{
	const char	chr = g->map.grid[col->cell.y][col->cell.x];
	const int	faded_offset = sizeof(g->texture) / sizeof(*g->texture) / 2;

	if (chr_is_wall(chr))
	{
		draw_wall_texture_set(g, col, col->cell);
		draw_wall_texture_set_pos(col);
	}
	else if (ft_memchr(CHAR_DOORS, chr, sizeof(CHAR_DOORS) - 1))
		draw_door_texture_set(g, col, chr);
	else if (chr == *CHAR_SECRET)
	{
		col->texture_id = door_find(g, col->cell)->secret_texture_id + \
			(col->dir == 'W' || col->dir == 'E') * faded_offset;
		draw_wall_texture_set_pos(col);
	}
}

void	draw_wall_texture_set(t_game *g, struct s_column *col, t_point pt)
{
	const int	faded_offset = sizeof(g->texture) / sizeof(*g->texture) / 2;

	if (col->dir == 'N' && g->map.grid[pt.y + 1][pt.x] == CHAR_DOORS_V[0])
		col->texture_id = TEXTURE_DOOR_1_W + faded_offset;
	else if (col->dir == 'N' && g->map.grid[pt.y + 1][pt.x] == CHAR_DOORS_V[1])
		col->texture_id = TEXTURE_DOOR_2_W + faded_offset;
	else if (col->dir == 'S' && g->map.grid[pt.y - 1][pt.x] == CHAR_DOORS_V[0])
		col->texture_id = TEXTURE_DOOR_1_W + faded_offset;
	else if (col->dir == 'S' && g->map.grid[pt.y - 1][pt.x] == CHAR_DOORS_V[1])
		col->texture_id = TEXTURE_DOOR_2_W + faded_offset;
	else if (col->dir == 'W' && g->map.grid[pt.y][pt.x + 1] == CHAR_DOORS_H[0])
		col->texture_id = TEXTURE_DOOR_1_W;
	else if (col->dir == 'W' && g->map.grid[pt.y][pt.x + 1] == CHAR_DOORS_H[1])
		col->texture_id = TEXTURE_DOOR_2_W;
	else if (col->dir == 'E' && g->map.grid[pt.y][pt.x - 1] == CHAR_DOORS_H[0])
		col->texture_id = TEXTURE_DOOR_1_W;
	else if (col->dir == 'E' && g->map.grid[pt.y][pt.x - 1] == CHAR_DOORS_H[1])
		col->texture_id = TEXTURE_DOOR_2_W;
	else
		col->texture_id = g->map.grid[pt.y][pt.x] - '0' + \
			(col->dir == 'W' || col->dir == 'E') * faded_offset;
}

void	draw_wall_texture_set_pos(struct s_column *col)
{
	if (col->dir == 'N')
		col->texture_pos = col->pos.x - (int)col->pos.x;
	else if (col->dir == 'E')
		col->texture_pos = col->pos.y - (int)col->pos.y;
	else if (col->dir == 'S')
		col->texture_pos = 1.0f - (col->pos.x - (int)col->pos.x);
	else if (col->dir == 'W')
		col->texture_pos = 1.0f - (col->pos.y - (int)col->pos.y);
}

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
	if (col->dir == 'W' || col->dir == 'E')
		col->texture_id += sizeof(game->texture) / sizeof(*game->texture) / 2;
}

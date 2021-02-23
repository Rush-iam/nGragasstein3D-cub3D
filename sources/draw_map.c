/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 18:52:51 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/23 20:57:10 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_map(t_game *game)
{
	unsigned	x;
	unsigned	y;

	img_clear_rgb(&game->map.img, 0xAA000000);
	y = 0;
	while (y < game->map.img.size.y)
	{
		x = 0;
		while (x < game->map.img.size.x)
		{
			draw_line(&game->map.img, (t_point){x, y},
					(t_point){x + MAP_SCALE, y}, 0x113322);
			draw_line(&game->map.img, (t_point){x, y},
					(t_point){x, y + MAP_SCALE}, 0x113322);
			if (game->map.grid[y / MAP_SCALE][x / MAP_SCALE] == '1')
				draw_square(&game->map.img, (t_point){
				x + MAP_SCALE / 2, y + MAP_SCALE / 2}, MAP_SCALE - 4, 0x33AA99);
			x += MAP_SCALE;
		}
		y += MAP_SCALE;
	}
	draw_map_player(game);
	draw_square(&game->map.img, (t_point){game->object[0].pos.x * MAP_SCALE,
							game->object[0].pos.y * MAP_SCALE}, 6, 0xFFFA80);
	mlx_put_image_to_window(game->mlx, game->win, game->map.img.ptr, 0, 0);
}

void	draw_map_player(t_game *game)
{
	unsigned	ray;

	draw_square(&game->map.img,
			(t_point){game->p.pos.x * MAP_SCALE,
						game->p.pos.y * MAP_SCALE}, 6, 0xFFFA80);
	ray = 0;
	while (ray < game->img.size.x)
	{
		draw_line(&game->map.img,
				(t_point){game->p.pos.x * MAP_SCALE, game->p.pos.y * MAP_SCALE},
				(t_point){game->column[ray].cell.x * MAP_SCALE,
						game->column[ray].cell.y * MAP_SCALE}, 0x888015);
		ray += 32;
	}
	draw_line(&game->map.img, (t_point){game->p.pos.x * MAP_SCALE,
										game->p.pos.y * MAP_SCALE}, (t_point){
		game->p.pos.x * MAP_SCALE + MAP_SCALE * cos(game->p.angle),
		game->p.pos.y * MAP_SCALE + MAP_SCALE * sin(game->p.angle)}, 0xFF4020);
}

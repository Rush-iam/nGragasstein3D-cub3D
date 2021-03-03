/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 18:52:51 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/03 19:15:40 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_map_init(t_game *game)
{
	int	null;

	game->map.img.size = (t_upoint){game->map.size.x * MAP_SCALE,
									game->map.size.y * MAP_SCALE};
	if (!(game->map.img.ptr = mlx_new_image(game->mlx, game->map.img.size.x,
														game->map.img.size.y)))
		terminate(game, ERR_MEM, "Memory allocation failed (draw map)");
	game->map.img.data = (unsigned *)mlx_get_data_addr(game->map.img.ptr, &null,
																&null, &null);
}

void	draw_map(t_game *game)
{
	t_point	pt;

	img_clear_rgb(&game->map.img, 0xAA000000);
	pt.y = 0;
	while (pt.y < (int)game->map.img.size.y)
	{
		pt.x = 0;
		while (pt.x < (int)game->map.img.size.x)
		{
			draw_line(&game->map.img, pt,
					(t_point){pt.x + MAP_SCALE, pt.y}, 0x113322);
			draw_line(&game->map.img, pt,
					(t_point){pt.x, pt.y + MAP_SCALE}, 0x113322);
			if (game->map.grid[pt.y / MAP_SCALE][pt.x / MAP_SCALE] == '1')
				draw_square(&game->map.img, (t_point){pt.x + MAP_SCALE / 2,
								pt.y + MAP_SCALE / 2}, MAP_SCALE - 4, 0x33AA99);
			pt.x += MAP_SCALE;
		}
		pt.y += MAP_SCALE;
	}
	draw_map_player(game);
	draw_map_objects(game);
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
				(t_point){game->column[ray]->cell.x * MAP_SCALE,
						game->column[ray]->cell.y * MAP_SCALE}, 0x888015);
		ray += 32;
	}
	draw_line(&game->map.img, (t_point){game->p.pos.x * MAP_SCALE,
										game->p.pos.y * MAP_SCALE}, (t_point){
		game->p.pos.x * MAP_SCALE + MAP_SCALE * cos(game->p.angle),
		game->p.pos.y * MAP_SCALE + MAP_SCALE * sin(game->p.angle)}, 0xFF4020);
}

void	draw_map_objects(t_game *game)
{
	t_list		*cur_list;
	t_object	*obj;

	cur_list = game->objects;
	while (cur_list)
	{
		obj = (t_object *)cur_list->content;
		draw_square(&game->map.img, (t_point){
				obj->pos.x * MAP_SCALE, obj->pos.y * MAP_SCALE}, 2, 0xFF7A40);
		cur_list = cur_list->next;
	}
}

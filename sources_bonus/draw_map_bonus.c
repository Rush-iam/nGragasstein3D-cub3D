/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:33:13 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/26 22:59:05 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_map_init(t_game *game)
{
	game->map.show = false;
	if (!img_create(game->mlx, &game->map.img,
		(t_upoint){game->map.size.x * MAP_SCALE, game->map.size.y * MAP_SCALE}))
		terminate(game, ERR_MLX, strerror(errno));
}

void	draw_map(t_game *game)
{
	t_point	pt;

	if (game->map.show == false)
		return ;
	img_clear_rgb(&game->map.img, 0xAA000000);
	pt.y = 0;
	while (pt.y < (int)game->map.img.size.y)
	{
		pt.x = 0;
		while (pt.x < (int)game->map.img.size.x)
		{
			draw_line(&game->map.img, pt,
					(t_point){pt.x + MAP_SCALE, pt.y}, COLOR_GREEN_FF);
			draw_line(&game->map.img, pt,
					(t_point){pt.x, pt.y + MAP_SCALE}, COLOR_GREEN_FF);
			if (ft_isdigit(game->map.grid[pt.y / MAP_SCALE][pt.x / MAP_SCALE]))
				draw_square(&game->map.img, (t_point){pt.x + MAP_SCALE / 2,
						pt.y + MAP_SCALE / 2}, MAP_SCALE - 4, COLOR_CYAN_F);
			pt.x += MAP_SCALE;
		}
		pt.y += MAP_SCALE;
	}
	draw_map_player(game);
	draw_map_objects(game);
	mlx_put_image_to_window(game->mlx, game->win, game->map.img.ptr, 0, 0);
}

void	draw_map_player(t_game *g)
{
	unsigned		ray;
	struct s_column	col;

	draw_square(&g->map.img,
	(t_point){g->p.pos.x * MAP_SCALE, g->p.pos.y * MAP_SCALE}, 6, COLOR_YELLOW);
	ray = 0;
	while (ray < g->img.size.x)
	{
		col = g->column[ray];
		draw_line(&g->map.img,
			(t_point){g->p.pos.x * MAP_SCALE, g->p.pos.y * MAP_SCALE},
			(t_point){col.pos.x * MAP_SCALE, col.pos.y * MAP_SCALE},
			COLOR_YELLOW_F);
		ray += 32;
	}
	draw_line(&g->map.img,
		   (t_point){g->p.pos.x * MAP_SCALE, g->p.pos.y * MAP_SCALE},
		   (t_point){MAP_SCALE * (g->p.pos.x + cos(g->p.angle)),
					MAP_SCALE * (g->p.pos.y + sin(g->p.angle))}, COLOR_ORANGE);
}

void	draw_map_objects(t_game *game)
{
	t_list		*cur_list;
	t_object	*obj;
	unsigned	color;

	cur_list = game->objects;
	while (cur_list)
	{
		obj = (t_object *)cur_list->content;
		if (obj->type == T_DECOR)
			color = MAP_COLOR_DECOR;
		else if (obj->type == T_ENEMY)
		{
			if (obj->e->state != ST_DEATH)
				color = MAP_COLOR_ENEMY;
			else
				color = MAP_COLOR_DECOR;
		}
		else
			color = MAP_COLOR_PICKUP;
		draw_square(&game->map.img, (t_point){
				obj->pos.x * MAP_SCALE, obj->pos.y * MAP_SCALE}, 2, color);
		cur_list = cur_list->next;
	}
}

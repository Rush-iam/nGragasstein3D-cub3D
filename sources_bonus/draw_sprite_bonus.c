/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprite_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 23:33:45 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/17 16:01:32 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_sprite(t_game *game, t_object *obj)
{
	int	start_x;
	int	cur_x;
	int	max_x;

	start_x = game->col_center + tan(obj->angle_from_p) / game->col_step -
														obj->size.x / 2;
	cur_x = start_x;
	max_x = cur_x + obj->size.x;
	cur_x = ft_max(cur_x, 0);
	max_x = ft_min(max_x, (int)game->img.size.x);
	while (cur_x < max_x)
	{
		if (obj->distance < game->column[cur_x]->distance)
		{
			if (obj->type == T_ENEMY && cur_x == (int)game->win_center.x)
				obj->e->targeted = true;
			draw_sprite_scaled(&game->img, obj, cur_x, (cur_x - start_x) /
								((double)obj->size.x / obj->sprite->size.x));
		}
		cur_x++;
	}
}

void	draw_sprite_scaled(t_img *img, t_object *obj, unsigned x,
						   unsigned src_x)
{
	const double	step = obj->sprite->size.y / obj->size.y;
	int				y;
	double			src_y;
	int				max_height;
	int				src_pixel;

	if (obj->size.y > img->size.y)
	{
		src_y = step * (obj->size.y - img->size.y) / 2;
		y = -1;
		max_height = img->size.y;
	}
	else
	{
		src_y = 0;
		y = -1 + (img->size.y - obj->size.y) / 2;
		max_height = y + obj->size.y;
	}
	while (++y < max_height)
	{
		if (((src_pixel = obj->sprite->data[(unsigned)src_y *
							obj->sprite->size.x + src_x]) & 0xFF000000) == 0)
			img->data[y * img->size.x + x] = src_pixel;
		src_y += step;
	}
}

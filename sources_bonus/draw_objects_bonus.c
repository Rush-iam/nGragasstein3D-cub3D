/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_objects_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 23:32:43 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/17 23:57:10 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_object_properties(t_game *game, t_object *obj)
{
	obj->render.size.x = game->col_scale / obj->distance;
	obj->render.size.y = obj->render.size.x * obj->sprite->aspect;
	obj->render.start.x = game->col_center + tan(obj->angle_from_p) /
							game->col_step - obj->render.size.x / 2;
	obj->render.start.y = (game->img.size.y - obj->render.size.y) / 2;
	obj->render.end.x = obj->render.start.x + obj->render.size.x;
	obj->render.end.y = obj->render.start.y + obj->render.size.y;
}

void	draw_objects(t_game *game, t_list *cur_list)
{
	t_object	*obj;

	while (cur_list)
	{
		obj = (t_object *)cur_list->content;
		if (0 < obj->render.end.x && obj->render.start.x < (int)game->img.size.x
			&& obj->distance > 0)
			draw_sprite(game, obj);
		cur_list = cur_list->next;
	}
}

void	draw_sprite(t_game *game, t_object *obj)
{
	t_point	min;
	t_point	max;

	min.x = ft_max(obj->render.start.x, 0);
	max.x = ft_min(obj->render.end.x, (int)game->img.size.x);
	while (obj->distance >= game->column[min.x]->distance && min.x < max.x)
		min.x++;
	while (obj->distance >= game->column[max.x - 1]->distance && min.x < max.x)
		max.x--;
	if (min.x == max.x)
		return ;
	min.y = ft_max((game->img.size.y - obj->render.size.y) / 2, 0);
	max.y = ft_min(min.y + obj->render.size.y, game->img.size.y);
	obj->render.step = (t_fpoint){obj->sprite->size.x / obj->render.size.x,
								  obj->sprite->size.y / obj->render.size.y};
	draw_sprite_scaled(&game->img, obj, min, max);
}

void	draw_sprite_scaled(t_img *img, t_object *obj, t_point min, t_point max)
{
	const double	x_src = obj->render.step.x * (min.x - obj->render.start.x);
	t_point 		cur;
	t_fpoint		cur_src;
	int				src_pixel;

	cur.y = min.y;
	if (obj->render.size.y > img->size.y)
		cur_src.y = obj->render.step.y * (obj->render.size.y - img->size.y) / 2;
	else
		cur_src.y = 0;
	while (cur.y < max.y)
	{
		cur.x = min.x;
		cur_src.x = x_src;
		while (cur.x < max.x)
		{
			if (((src_pixel = obj->sprite->data[(unsigned)cur_src.y *
				obj->sprite->size.x + (unsigned)cur_src.x]) & 0xFF000000) == 0)
				img->data[cur.y * img->size.x + cur.x] = src_pixel;
			cur_src.x += obj->render.step.x;
			cur.x++;
		}
		cur_src.y += obj->render.step.y;
		cur.y++;
	}
}

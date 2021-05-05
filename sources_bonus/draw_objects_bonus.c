/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_objects_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 23:32:43 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/05 16:56:04 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_object_properties(t_game *game, t_object *obj)
{
	obj->angle_to_p = obj->atan_diff - game->p.angle;
	if (obj->angle_to_p < -M_PI_F)
		obj->angle_to_p += PI2_F;
	if (obj->distance > 0.1f)
	{
		obj->render.size.y = (int)(game->col_scale / obj->distance) & ~1;
		obj->render.size.x = (int) \
							(obj->render.size.y * obj->sprite->aspect) & ~1;
		obj->render.start_0 = game->center.x + tanf(obj->angle_to_p) / \
									game->col_step - obj->render.size.x / 2;
		obj->render.start.x = obj->render.start_0 + \
								obj->render.size.x * obj->sprite->min_x;
		obj->render.end.x = obj->render.start_0 + \
								obj->render.size.x * obj->sprite->max_x;
		obj->render.start.y = game->horizon + \
							obj->render.size.y * (game->z_level - 1.0f);
		obj->render.end.y = obj->render.start.y + obj->render.size.y;
	}
}

void	draw_objects(t_game *game)
{
	t_list		*cur_list;
	t_object	*obj;

	ft_lstsort(&game->objects, objects_sort);
	cur_list = game->objects;
	while (cur_list)
	{
		obj = (t_object *)cur_list->content;
		draw_object_properties(game, obj);
		if (0 < obj->render.end.x && obj->render.start.x < (int)game->img.size.x
			&& obj->distance > 0.1f)
			draw_sprite(game, obj);
		cur_list = cur_list->next;
	}
}

void	draw_sprite(t_game *game, t_object *obj)
{
	t_point		min;
	t_point		max;

	min.x = ft_max(obj->render.start.x, 0);
	max.x = ft_min(obj->render.end.x, (int)game->img.size.x);
	while (min.x < max.x && obj->distance >= game->column[min.x].distance)
		min.x++;
	while (min.x < max.x && obj->distance >= game->column[max.x - 1].distance)
		max.x--;
	if (min.x == max.x)
		return ;
	min.y = ft_max(0, obj->render.start.y);
	max.y = ft_min(game->img.size.y, obj->render.end.y);
	obj->render.step = (t_fpoint){
		(float)obj->sprite->size.x / obj->render.size.x,
		(float)obj->sprite->size.y / obj->render.size.y};
	if (obj->distance < 1.5f * game->fade_distance)
		draw_sprite_scaled(game, obj, min, max);
	else
		draw_sprite_scaled_f(game, obj, min, max);
}

void	draw_sprite_scaled(t_game *g, t_object *obj, t_point min, t_point max)
{
	const float	x_src = obj->render.step.x * (min.x - obj->render.start_0);
	int			x;
	t_fpoint	cur_src;
	int			src_pixel;

	cur_src.y = fmaxf(0.0f, -obj->render.start.y * obj->render.step.y);
	while (min.y < max.y)
	{
		x = min.x;
		cur_src.x = x_src;
		if (obj->sprite->alpha_y[(uint32_t)cur_src.y] == false)
		{
			while (x < max.x)
			{
				src_pixel = obj->sprite->data[(uint32_t)cur_src.y * \
									obj->sprite->size.x + (uint32_t)cur_src.x];
				if ((src_pixel >> 24) == 0)
					g->img.data[min.y * g->img.size.x + x] = src_pixel;
				cur_src.x += obj->render.step.x;
				x++;
			}
		}
		cur_src.y += obj->render.step.y;
		min.y++;
	}
}

void	draw_sprite_scaled_f(t_game *g, t_object *obj, t_point min, t_point max)
{
	const float	x_src = obj->render.step.x * (min.x - obj->render.start_0);
	int			x;
	t_fpoint	cur_src;
	int			pix;
	const float	f = 1.5f * g->fade_distance / obj->distance;

	cur_src.y = fmaxf(0.0f, -obj->render.start.y * obj->render.step.y);
	while (min.y < max.y)
	{
		x = min.x - 1;
		cur_src.x = x_src;
		if (obj->sprite->alpha_y[(uint32_t)cur_src.y] == false)
		{
			while (++x < max.x)
			{
				pix = obj->sprite->data[(uint32_t)cur_src.y * \
						obj->sprite->size.x + (uint32_t)cur_src.x];
				if ((pix >> 24) == 0)
					g->img.data[min.y * g->img.size.x + x] = pixel_fade(pix, f);
				cur_src.x += obj->render.step.x;
			}
		}
		cur_src.y += obj->render.step.y;
		min.y++;
	}
}

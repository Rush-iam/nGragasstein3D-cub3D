/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprites.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 16:34:37 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/04 23:50:25 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		objects_sort(t_object *obj1, t_object *obj2)
{
	return (obj1->distance < obj2->distance);
}

void	draw_objects(t_game *game)
{
	t_list		*cur_list;
	t_object	*obj;
	double		angle;
	t_fpoint	diff;

	ft_lstsort(&game->objects, objects_sort);
	cur_list = game->objects;
	while (cur_list)
	{
		obj = (t_object *)cur_list->content;
		diff = (t_fpoint){obj->pos.x - game->p.pos.x,
						obj->pos.y - game->p.pos.y};
		if ((obj->distance = game->p.cossin.x * diff.x +
				game->p.cossin.y * diff.y) > 0.2)
		{
			angle = atan2(diff.y, diff.x);
			if (fabs(game->p.angle - angle - PI2) <= M_PI)
				angle += PI2;
			angle -= game->p.angle;
			if (fabs(angle) < REAL_FOV)
				draw_sprite(game, obj, angle);
		}
		cur_list = cur_list->next;
	}
}

void	draw_sprite(t_game *game, t_object *obj, double angle)
{
	int		start_ray;
	int		cur;
	int		max_ray;
	double	cur_angle;

	obj->size.x = game->img.size.x / COL_SCALE / obj->distance;
	obj->size.y = obj->size.x * obj->sprite->aspect;
	start_ray = (angle / REAL_FOV + .5) * game->img.size.x;
	cur_angle = atan(tan(FOV / (game->img.size.x - 1)) *
									(start_ray - game->img.size.x / 2.));
	start_ray = ((angle - (cur_angle - angle)) / REAL_FOV + .5) *
			game->img.size.x - obj->size.x / 2;
	cur = start_ray;
	max_ray = cur + obj->size.x;
	if (cur < 0)
		cur = 0;
	if (max_ray >= (int)game->img.size.x)
		max_ray = (int)game->img.size.x;
	while (cur < max_ray)
	{
		if (obj->distance < game->column[cur]->distance)
			draw_sprite_scaled(&game->img, obj, cur,
			(cur - start_ray) / ((double)obj->size.x / obj->sprite->size.x));
		cur++;
	}
}

void	draw_sprite_scaled(t_img *img, t_object *obj, unsigned x,
																unsigned src_x)
{
	const double	step = (double)obj->sprite->size.y / obj->size.y;
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

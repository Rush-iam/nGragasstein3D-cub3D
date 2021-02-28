/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprites.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 16:34:37 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/28 18:36:56 by ngragas          ###   ########.fr       */
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
		angle = atan2(diff.y, diff.x);
		if (fabs(game->p.angle - angle - PI2) <= M_PI)
			angle += PI2;
		angle -= game->p.angle;
		obj->distance = game->p.cossin.x * diff.x + game->p.cossin.y * diff.y;
		if (fabs(angle) < FOV)
		{
			obj->width = game->img.size.x / COL_SCALE /
								hypot(diff.x, diff.y) * obj->sprite->aspect;
			draw_sprite(game, obj, angle);
		}
		cur_list = cur_list->next;
	}
}

void	draw_sprite(t_game *game, t_object *obj, double angle)
{
	int				start_ray;
	int				cur;
	int				max_ray;
	const double	scale_x = obj->width / obj->sprite->size.x;

	obj->height = game->img.size.x / COL_SCALE / obj->distance;
	start_ray = (angle / FOV + 0.5) * game->img.size.x - obj->width / 2;
	cur = start_ray;
	max_ray = cur + obj->width;
	if (cur < 0)
		cur = 0;
	if (max_ray >= (int)game->img.size.x)
		max_ray = (int)game->img.size.x;
	while (cur < max_ray)
	{
		if (obj->distance < game->column[cur]->distance)
			draw_sprite_scaled(
					&game->img, obj, cur, (cur - start_ray) / scale_x);
		cur++;
	}
}

void	draw_sprite_scaled(t_img *img, t_object *obj, unsigned x,
																unsigned src_x)
{
	const double	step = (double)obj->sprite->size.y / obj->height;
	int				y;
	double			src_y;
	int				max_height;
	int				src_pixel;

	if (obj->height > img->size.y)
	{
		src_y = step * (obj->height - img->size.y) / 2;
		y = -1;
		max_height = img->size.y;
	}
	else
	{
		src_y = 0;
		y = -1 + (img->size.y - obj->height) / 2;
		max_height = y + obj->height;
	}
	while (++y < max_height)
	{
		if (((src_pixel = obj->sprite->data[(unsigned)src_y *
						obj->sprite->size.x + src_x]) & 0xFF000000) == 0)
			img->data[y * img->size.x + x] = src_pixel;
		src_y += step;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprites.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 16:34:37 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/05 17:09:03 by ngragas          ###   ########.fr       */
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
		if ((obj->distance = game->p.vect.x * diff.x +
							 game->p.vect.y * diff.y) > 0.01)
		{
			angle = atan2(diff.y, diff.x);
			if (fabs(game->p.angle - angle - PI2) <= M_PI)
				angle += PI2;
			angle -= game->p.angle;
			if (fabs(angle) < game->fov + M_PI_4)
				draw_sprite(game, obj, angle);
		}
		cur_list = cur_list->next;
	}
}

void	draw_sprite(t_game *game, t_object *obj, double angle)
{
	int	start_x;
	int	cur_x;
	int	max_x;

	obj->size.x = game->col_scale / obj->distance;
	obj->size.y = obj->size.x * obj->sprite->aspect;
	start_x = game-> + tan(angle) / game->col_step - obj->size.x / 2;
	cur_x = start_x;
	max_x = cur_x + obj->size.x;
	if (cur_x < 0)
		cur_x = 0;
	if (max_x >= (int)game->img.size.x)
		max_x = (int)game->img.size.x;
	while (cur_x < max_x)
	{
		if (obj->distance < game->column[cur_x]->distance)
			draw_sprite_scaled(&game->img, obj, cur_x,
			(cur_x - start_x) / ((double)obj->size.x / obj->sprite->size.x));
		cur_x++;
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprites.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 16:34:37 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/22 18:51:00 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	objects_sort(t_game *game)
{
	unsigned	i;
	bool		sorted;
	t_object	tmp;

	if (game->object_count == 0)
		return ;
	sorted = false;
	while (sorted == false)
	{
		sorted = true;
		i = 0;
		while (i < game->object_count - 1)
		{
			if (game->object[i].distance < game->object[i + 1].distance)
			{
				sorted = false;
				tmp = game->object[i];
				game->object[i] = game->object[i + 1];
				game->object[i + 1] = tmp;
			}
			i++;
		}
	}
}

void	draw_objects(t_game *game)
{
	unsigned	i;
	double		angle;
	t_fpoint	diff;

	objects_sort(game);
	i = 0;
	while (i < game->object_count)
	{
		diff = (t_fpoint){game->object[i].pos.x - game->p.pos.x,
						game->object[i].pos.y - game->p.pos.y};
		angle = atan2(diff.y, diff.x);
		if (fabs(game->p.angle - angle - PI2) <= M_PI)
			angle += PI2;
		angle -= game->p.angle;
		game->object[i].distance = cos(game->p.angle) * diff.x +
									sin(game->p.angle) * diff.y;
		if (fabs(angle) < FOV)
		{
			game->object[i].height = COL_SCALE / game->object[i].distance;
			game->object[i].width = COL_SCALE / sqrt(pow(diff.x, 2) +
							pow(diff.y, 2)) * game->object[i].sprite->aspect;
			draw_sprite(game, &game->object[i], angle);
		}
		i++;
	}
}

void	draw_sprite(t_game *game, t_object *obj, double angle)
{
	int				start_ray;
	int				cur;
	int				max_ray;
	const double	scale_x = obj->width / obj->sprite->img.size.x;

	start_ray = (angle / FOV + 0.5) * game->img.size.x - obj->width / 2;
	cur = start_ray;
	max_ray = cur + obj->width;
	if (cur < 0)
		cur = 0;
	if (max_ray >= (int)game->img.size.x)
		max_ray = (int)game->img.size.x;
	while (cur < max_ray)
	{
		if (obj->distance < game->column[cur].distance)
			draw_sprite_scaled(game, obj, cur, (cur - start_ray) / scale_x);
		cur++;
	}
}

void	draw_sprite_scaled(t_game *game, t_object *obj, int x, unsigned src_x)
{
	const double	step = (double)obj->sprite->img.size.y / obj->height;
	int				y;
	double			src_y;
	int				max_height;
	int				src_pixel;

	if (obj->height > game->img.size.y)
	{
		src_y = step * (obj->height - game->img.size.y) / 2;
		y = -1;
		max_height = game->img.size.y;
	}
	else
	{
		src_y = 0;
		y = -1 + (game->img.size.y - obj->height) / 2;
		max_height = y + obj->height;
	}
	while (++y < max_height)
	{
		if (((src_pixel = obj->sprite->img.data[(unsigned)src_y *
						obj->sprite->img.size.x + src_x]) & 0xFF000000) == 0)
			game->img.data[y * game->img.size.x + x] = src_pixel;
		src_y += step;
	}
}

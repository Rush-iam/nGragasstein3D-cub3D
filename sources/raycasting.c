/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 20:04:36 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/06 22:38:52 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void		ray_cast(t_game *game)
{
	unsigned	ray;
	double		angle;

	ray = 0;
	while (ray < game->img.size.x)
	{
		angle = game->p.angle + atan(game->col_step * (ray - game->col_center));
		if (angle < 0)
			angle += PI2;
		else if (angle > PI2)
			angle -= PI2;
		ray_intersect(game, angle, ray);
		ray++;
	}
}

void		ray_intersect(t_game *game, double cur_angle, unsigned ray)
{
	t_fpoint		x1;
	t_fpoint		y1;
	t_fpoint		distance;
	const double	tan_cur_angle = tan(cur_angle);

	x1 = (cur_angle <= M_PI_2 || cur_angle > 3 * M_PI_2) ?
		ray_intersect_x(game, (t_fpoint){1, tan_cur_angle}) :
		ray_intersect_x(game, (t_fpoint){-1, -tan_cur_angle});
	y1 = (cur_angle <= M_PI) ?
		ray_intersect_y(game, (t_fpoint){1 / tan_cur_angle, 1}) :
		ray_intersect_y(game, (t_fpoint){-1 / tan_cur_angle, -1});
	distance.x = game->p.vector.x * (x1.x - game->p.pos.x) +
				 game->p.vector.y * (x1.y - game->p.pos.y);
	distance.y = game->p.vector.x * (y1.x - game->p.pos.x) +
				 game->p.vector.y * (y1.y - game->p.pos.y);
	if (distance.x < distance.y)
		*game->column[ray] = (struct s_column)
			{distance.x, game->col_scale / distance.x,
				x1.y - (int)x1.y, (x1.x < game->p.pos.x) ? 'W' : 'E'};
	else
		*game->column[ray] = (struct s_column)
			{distance.y, game->col_scale / distance.y,
				y1.x - (int)y1.x, (y1.y < game->p.pos.y) ? 'N' : 'S'};
	if (game->column[ray]->dir == 'W' || game->column[ray]->dir == 'S')
		game->column[ray]->texture_pos = 1. - game->column[ray]->texture_pos;
}

t_fpoint	ray_intersect_x(t_game *game, t_fpoint step)
{
	t_fpoint	check;

	check.x = (int)game->p.pos.x + (step.x > 0) - (step.x < 0);
	if (step.x > 0)
		check.y = game->p.pos.y + step.y *
								(1 - (game->p.pos.x - (int)game->p.pos.x));
	else
		check.y = game->p.pos.y + step.y * (game->p.pos.x - (int)game->p.pos.x);
	while ((unsigned)check.y < game->map.size.y &&
			(unsigned)check.x < game->map.size.x &&
			game->map.grid[(unsigned)check.y][(unsigned)check.x] != '1')
		check = (t_fpoint){check.x + step.x, check.y + step.y};
	check.x += (step.x < 0);
	return (check);
}

t_fpoint	ray_intersect_y(t_game *game, t_fpoint step)
{
	t_fpoint	check;

	check.y = (int)game->p.pos.y + (step.y > 0) - (step.y < 0);
	if (step.y > 0)
		check.x = game->p.pos.x + step.x *
								(1 - (game->p.pos.y - (int)game->p.pos.y));
	else
		check.x = game->p.pos.x + step.x * (game->p.pos.y - (int)game->p.pos.y);
	while ((unsigned)check.y < game->map.size.y &&
			(unsigned)check.x < game->map.size.x &&
			game->map.grid[(unsigned)check.y][(unsigned)check.x] != '1')
		check = (t_fpoint){check.x + step.x, check.y + step.y};
	check.y += (step.y < 0);
	return (check);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:45 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/06 17:32:45 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

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

	x1 = (cur_angle < M_PI_2 || cur_angle > 3 * M_PI_2) ?
		 ray_intersect_x(game, (t_fpoint){1, tan_cur_angle}) :
		 ray_intersect_x(game, (t_fpoint){-1, -tan_cur_angle});
	y1 = (cur_angle < M_PI) ?
		 ray_intersect_y(game, (t_fpoint){1 / tan_cur_angle, 1}) :
		 ray_intersect_y(game, (t_fpoint){-1 / tan_cur_angle, -1});
	distance.x = game->p.cossin.x * (x1.x - game->p.pos.x) +
				 game->p.cossin.y * (x1.y - game->p.pos.y);
	distance.y = game->p.cossin.x * (y1.x - game->p.pos.x) +
				 game->p.cossin.y * (y1.y - game->p.pos.y);
	if (distance.x < distance.y)
		*game->column[ray] = (struct s_column)
				{distance.x, game->col_scale / distance.x, x1,
				 x1.y - (int)x1.y, (x1.x < game->p.pos.x) ? 'W' : 'E'};
	else
		*game->column[ray] = (struct s_column)
				{distance.y, game->col_scale / distance.y, y1,
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

/*
** == ping-pong intersect detection (0.5% faster?)
**void	ray_intersect(t_game *game, double cur_angle, unsigned ray)
**{
**	t_fpoint	x_axis;
**	t_fpoint	y_axis;
**	t_fpoint	x_step;
**	t_fpoint	y_step;
**	t_fpoint	dist;
**	t_fpoint	dist_step;
**
**	x_step = (cur_angle < M_PI_2 || cur_angle > 3 * M_PI_2) ?
**		(t_fpoint){1, tan(cur_angle)} : (t_fpoint){-1, -tan(cur_angle)};
**	x_axis.x = (int)game->p.pos.x + (x_step.x > 0) - (x_step.x < 0);
**	x_axis.y = game->p.pos.y + x_step.y * (x_step.x > 0 ? 1 - (game->p.pos.x
**			- (int)game->p.pos.x) : game->p.pos.x - (int)game->p.pos.x);
**	y_step = (cur_angle < M_PI) ?
**		(t_fpoint){1 / tan(cur_angle), 1} : (t_fpoint){-1 / tan(cur_angle), -1};
**	y_axis.y = (int)game->p.pos.y + (y_step.y > 0) - (y_step.y < 0) ;
**	y_axis.x = game->p.pos.x + y_step.x * (y_step.y > 0 ? 1 - (game->p.pos.y
**			- (int)game->p.pos.y) : game->p.pos.y - (int)game->p.pos.y);
**	dist_step.x = cos(game->p.angle) * x_step.x + sin(game->p.angle) * x_step.y;
**	dist_step.y = cos(game->p.angle) * y_step.x + sin(game->p.angle) * y_step.y;
**	dist.x = cos(game->p.angle) * (x_axis.x - game->p.pos.x + (x_step.x < 0)) +
**			 sin(game->p.angle) * (x_axis.y - game->p.pos.y);
**	dist.y = cos(game->p.angle) * (y_axis.x - game->p.pos.x) +
**			 sin(game->p.angle) * (y_axis.y - game->p.pos.y + (y_step.y < 0));
**	while (true)
**	{
**		if (dist.x < dist.y)
**		{
**			if ((unsigned)x_axis.y >= game->map.size.y ||
**				(unsigned)x_axis.x >= game->map.size.x ||
**				game->map.grid[(int)x_axis.y][(int)x_axis.x] == '1')
**			{
**				x_axis.x += (x_step.x < 0);
**				*game->column[ray] = (struct s_column){dist.x,
**					game->img.size.x / COL_SCALE / dist.x,
**					x_axis, x_axis.y - (int)x_axis.y,
**					"EW"[x_axis.x >= game->p.pos.x]};
**				return ;
**			}
**			x_axis = (t_fpoint){x_axis.x + x_step.x, x_axis.y + x_step.y};
**			dist.x += dist_step.x;
**		}
**		else
**		{
**			if ((unsigned)y_axis.y >= game->map.size.y ||
**				(unsigned)y_axis.x >= game->map.size.x ||
**				game->map.grid[(int)y_axis.y][(int)y_axis.x] == '1')
**			{
**				y_axis.y += (y_step.y < 0);
**				*game->column[ray] = (struct s_column){dist.y,
**					game->img.size.x / COL_SCALE / dist.y,
**					y_axis, y_axis.x - (int)y_axis.x,
**					"NS"[y_axis.y >= game->p.pos.y]};
**				return ;
**			}
**			y_axis = (t_fpoint){y_axis.x + y_step.x, y_axis.y + y_step.y};
**			dist.y += dist_step.y;
**		}
**	}
**}
*/

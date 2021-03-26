/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:45 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/26 23:43:23 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void		ray_cast(t_game *g)
{
	int		ray;
	float	angle;

	ray = 0;
	while (ray < (int)g->img.size.x)
	{
		angle = g->p.angle +
				atanf(g->col_step * (ray - (int)g->win_center.x));
		if (angle < -M_PI_F)
			angle += PI2_F;
		else if (angle >= M_PI_F)
			angle -= PI2_F;
		g->column[ray] = ray_intersect(g, angle);
		if ((unsigned)g->column[ray].cell.x < g->map.size.x &&
			(unsigned)g->column[ray].cell.y < g->map.size.y)
			draw_wall_texture_set(g, &g->column[ray], g->column[ray].cell);
		ray++;
	}
}

struct s_column	ray_intersect(t_game *game, float angle)
{
	t_fpoint		x1;
	t_fpoint		y1;
	t_fpoint		distance;
	const t_point	negative = {angle < -M_PI_2 || angle >= M_PI_2, angle < 0};
	const float		tan_cur_angle = tanf(angle);

	x1 = negative.x == true ?
		 ray_intersect_x(game, game->p.pos, -1, -tan_cur_angle) :
		 ray_intersect_x(game, game->p.pos, 1, tan_cur_angle);
	y1 = negative.y == false ?
		 ray_intersect_y(game, game->p.pos, 1 / tan_cur_angle, 1) :
		 ray_intersect_y(game, game->p.pos, -1 / tan_cur_angle, -1);
	distance.x = game->p.vect.x * (x1.x - game->p.pos.x) +
				 game->p.vect.y * (x1.y - game->p.pos.y);
	distance.y = game->p.vect.x * (y1.x - game->p.pos.x) +
				 game->p.vect.y * (y1.y - game->p.pos.y);
	if (distance.x < distance.y)
		return ((struct s_column){distance.x, game->col_scale / distance.x,
				(t_point){x1.x - negative.x, x1.y}, -1U, x1.y - (int)x1.y,
				"EW"[x1.x < game->p.pos.x]});
	else
		return ((struct s_column){distance.y, game->col_scale / distance.y,
				(t_point){y1.x, y1.y - negative.y}, -1U, y1.x - (int)y1.x,
				"SN"[y1.y < game->p.pos.y]});
}

t_fpoint	ray_intersect_x(t_game *game, t_fpoint from, int step_x, float step_y)
{
	t_fpoint	check;

	check.x = (int)from.x + (step_x > 0) - (step_x < 0);
	if (step_x > 0)
		check.y = from.y + step_y * (1.0f - (from.x - (int)from.x));
	else
		check.y = from.y + step_y * (from.x - (int)from.x);
	while ((unsigned)check.y < game->map.size.y &&
			(unsigned)check.x < game->map.size.x)
	{
		if (ft_isdigit(game->map.grid[(unsigned)check.y][(unsigned)check.x]))
			break ;
		else if (ft_memchr(CHAR_DOORS, game->map.grid[(unsigned)check.y]
									[(unsigned)check.x], sizeof(CHAR_DOORS)))
			return (ray_intersect_x_door(game, check, step_x, step_y));
		check = (t_fpoint){check.x + step_x, check.y + step_y};
	}
	check.x += (step_x < 0);
	return (check);
}

t_fpoint	ray_intersect_x_door(t_game *game, t_fpoint check, int step_x, float step_y)
{
	t_fpoint	check_door;
	float		position;

	check_door = (t_fpoint){check.x + step_x / 2.0f, check.y + step_y / 2.0f};
	position = check_door.y - (unsigned)check.y;
	if (0 < position &&
		position < door_find(game, (t_point){check.x, check.y})->part_opened)
		return (ray_intersect_x(game, (t_fpoint){
				check_door.x + (step_x < 0), check_door.y}, step_x, step_y));
	return ((t_fpoint){check_door.x + (step_x < 0), check_door.y});
}

t_fpoint ray_intersect_y(t_game *game, t_fpoint from, float step_x, int step_y)
{
	t_fpoint	check;

	check.y = (int)from.y + (step_y > 0) - (step_y < 0);
	if (step_y > 0)
		check.x = from.x + step_x * (1.0f - (from.y - (int)from.y));
	else
		check.x = from.x + step_x * (from.y - (int)from.y);
	while ((unsigned)check.y < game->map.size.y &&
			(unsigned)check.x < game->map.size.x)
	{
		if (ft_isdigit(game->map.grid[(unsigned)check.y][(unsigned)check.x]))
			break ;
		else if (ft_memchr(CHAR_DOORS, game->map.grid[(unsigned)check.y]
									[(unsigned)check.x], sizeof(CHAR_DOORS)))
			return (ray_intersect_y_door(game, check, step_x, step_y));
		check = (t_fpoint){check.x + step_x, check.y + step_y};
	}
	check.y += (step_y < 0);
	return (check);
}

t_fpoint	ray_intersect_y_door(t_game *game, t_fpoint check, float step_x, int step_y)
{
	t_fpoint	check_door;
	float		position;

	check_door = (t_fpoint){check.x + step_x / 2.0f, check.y + (step_y < 0) + step_y / 2.0f};
	position = check_door.x - (unsigned)check.x;
	if (0 < position &&
		position < door_find(game, (t_point){check.x, check.y})->part_opened)
		return (ray_intersect_y(game, (t_fpoint){
				check_door.x, check_door.y}, step_x, step_y));
	return ((t_fpoint){check_door.x, check_door.y});
}

float	ray_intersect_distance(t_game *game, float cur_angle)
{
	t_fpoint	x1;
	t_fpoint	y1;
	t_fpoint	distance;
	const float	tan_cur_angle = tanf(cur_angle);

	x1 = (cur_angle < -M_PI_2_F || cur_angle >= M_PI_2_F) ?
		 ray_intersect_x(game, game->p.pos, -1, -tan_cur_angle) :
		 ray_intersect_x(game, game->p.pos, 1, tan_cur_angle);
	y1 = (cur_angle >= 0) ?
		 ray_intersect_y(game, game->p.pos, 1 / tan_cur_angle, 1) :
		 ray_intersect_y(game, game->p.pos, -1 / tan_cur_angle, -1);
	distance.x = hypotf(x1.x - game->p.pos.x, x1.y - game->p.pos.y);
	distance.y = hypotf(y1.x - game->p.pos.x, y1.y - game->p.pos.y);
	return (fmin(distance.x, distance.y));
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

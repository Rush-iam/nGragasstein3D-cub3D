/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:45 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/27 18:32:43 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void		ray_cast(t_game *g)
{
	int				ray;
	float			angle;
	struct s_column	*col;

	ray = 0;
	while (ray < (int)g->img.size.x)
	{
		col = &g->column[ray];
		angle = g->p.angle + g->angles[ray];
		*col = ray_intersect(g, tanf(angle),
		(t_point){angle < -M_PI_2_F || angle >= M_PI_2_F, sinf(angle) < 0.0f});
		if ((unsigned)col->cell.x < g->map.size.x &&
			(unsigned)col->cell.y < g->map.size.y)
		{
			if (ft_isdigit(col->chr = g->map.grid[col->cell.y][col->cell.x]))
				draw_wall_texture_set(g, col, col->cell);
			else if (ft_memchr(CHAR_DOORS, col->chr, sizeof(CHAR_DOORS)))
				draw_door_texture_set(g, col);
		}
		ray++;
	}
}

struct s_column	ray_intersect(t_game *game, float tan_cur_angle, t_point negative)
{
	t_ray		x1;
	t_ray		y1;
	t_fpoint	distance;

	x1 = (negative.x == true) ?
		 ray_intersect_x(game, -1, -tan_cur_angle) :
		 ray_intersect_x(game, 1, tan_cur_angle);
	y1 = (negative.y == false) ?
		 ray_intersect_y(game, 1.0f / tan_cur_angle, 1) :
		 ray_intersect_y(game, 1.0f / -tan_cur_angle, -1);
	distance.x = game->p.vect.x * (x1.pos.x - game->p.pos.x) +
				 game->p.vect.y * (x1.pos.y - game->p.pos.y);
	distance.y = game->p.vect.x * (y1.pos.x - game->p.pos.x) +
				 game->p.vect.y * (y1.pos.y - game->p.pos.y);
	if (distance.x < distance.y)
		return ((struct s_column){distance.x, game->col_scale / distance.x,
				x1.pos, x1.cell, '\0', -1U, x1.pos.y - (int)x1.pos.y,
				"EW"[x1.pos.x < game->p.pos.x]});
	else
		return ((struct s_column){distance.y, game->col_scale / distance.y,
				y1.pos, y1.cell, '\0', -1U, y1.pos.x - (int)y1.pos.x,
				"SN"[y1.pos.y < game->p.pos.y]});
}

t_ray	ray_intersect_x(t_game *g, int step_x, float step_y)
{
	int		x;
	float	y;
	char	chr;

	x = (int)g->p.pos.x + step_x;
	if (step_x > 0)
		y = g->p.pos.y + step_y * (1.0f - (g->p.pos.x - (int)g->p.pos.x));
	else
		y = g->p.pos.y + step_y * (g->p.pos.x - (int)g->p.pos.x);
	while ((unsigned)y < g->map.size.y && (unsigned)x < g->map.size.x)
	{
		chr = g->map.grid[(int)y][x];
		if (ft_isdigit(chr))
			break ;
		if (ft_memchr(CHAR_DOORS, chr, sizeof(CHAR_DOORS)) &&
			!(y + step_y / 2.0f > (int)y && y + step_y / 2.0f - (int)y <
			  door_find(g, (t_point){x, y})->part_opened))
			return ((t_ray){(t_fpoint){x + step_x / 2.0f + (step_x < 0),
							y + step_y / 2.0f}, (t_point){x, y}});
		x += step_x;
		y += step_y;
	}
	return ((t_ray){(t_fpoint){x + (step_x < 0), y},(t_point){x, y}});
}

t_ray	ray_intersect_y(t_game *g, float step_x, int step_y)
{
	float	x;
	int		y;
	char	chr;

	y = (int)g->p.pos.y + step_y;
	if (step_y > 0)
		x = g->p.pos.x + step_x * (1.0f - (g->p.pos.y - (int)g->p.pos.y));
	else
		x = g->p.pos.x + step_x * (g->p.pos.y - (int)g->p.pos.y);
	while ((unsigned)y < g->map.size.y && (unsigned)x < g->map.size.x)
	{
		chr = g->map.grid[y][(unsigned)x];
		if (ft_isdigit(chr))
			break;
		if (ft_memchr(CHAR_DOORS, chr, sizeof(CHAR_DOORS)) &&
			!(x + step_x / 2.0f > (int)x && x + step_x / 2.0f - (int)x <
				door_find(g, (t_point){x, y})->part_opened))
			return ((t_ray){(t_fpoint){x + step_x / 2.0f,
					y + step_y / 2.0f + (step_y < 0)}, (t_point){x, y}});
		x += step_x;
		y += step_y;
	}
	return ((t_ray){(t_fpoint){x, y + (step_y < 0)}, (t_point){x, y}});
}

float	ray_intersect_distance(t_game *game, float cur_angle)
{
	t_ray		x1;
	t_ray		y1;
	t_fpoint	distance;
	const float	tan_cur_angle = tanf(cur_angle);

	x1 = (cur_angle < -M_PI_2_F || cur_angle >= M_PI_2_F) ?
		 ray_intersect_x(game, -1, -tan_cur_angle) :
		 ray_intersect_x(game, 1, tan_cur_angle);
	y1 = (cur_angle >= 0) ?
		 ray_intersect_y(game, 1 / tan_cur_angle, 1) :
		 ray_intersect_y(game, -1 / tan_cur_angle, -1);
	distance.x = hypotf(x1.pos.x - game->p.pos.x, x1.pos.y - game->p.pos.y);
	distance.y = hypotf(y1.pos.x - game->p.pos.x, y1.pos.y - game->p.pos.y);
	return (fminf(distance.x, distance.y));
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

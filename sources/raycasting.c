/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 20:04:36 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/02 20:25:06 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	ray_cast(t_game *game)
{
	unsigned int	ray;
	double			angle;

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

void	ray_intersect(t_game *g, double angle, unsigned int ray)
{
	const int		stx = 1 - 2 * !(angle <= M_PI_2 || angle > 3 * M_PI_2);
	const int		sty = 1 - 2 * (angle > M_PI);
	const t_fpoint	x1 = ray_intersect_x(g, (t_fpoint){stx, stx * tan(angle)});
	const t_fpoint	y1 = ray_intersect_y(g, (t_fpoint){sty / tan(angle), sty});
	const t_fpoint	distance = {g->p.vect.x * (x1.x - g->p.pos.x) + \
								g->p.vect.y * (x1.y - g->p.pos.y),
								g->p.vect.x * (y1.x - g->p.pos.x) + \
								g->p.vect.y * (y1.y - g->p.pos.y)};

	if (distance.x < distance.y)
		g->column[ray] = (struct s_column) \
			{distance.x, g->col_scale / distance.x, \
				x1.y - (int)x1.y, "EW"[x1.x < g->p.pos.x]};
	else
		g->column[ray] = (struct s_column) \
			{distance.y, g->col_scale / distance.y, \
				y1.x - (int)y1.x, "SN"[y1.y < g->p.pos.y]};
	if (g->column[ray].dir == 'W' || g->column[ray].dir == 'S')
		g->column[ray].texture_pos = 1. - g->column[ray].texture_pos;
}

t_fpoint	ray_intersect_x(t_game *game, t_fpoint step)
{
	t_fpoint	check;

	check.x = (int)game->p.pos.x + (step.x > 0) - (step.x < 0);
	if (step.x > 0)
		check.y = game->p.pos.y + step.y * \
								(1 - (game->p.pos.x - (int)game->p.pos.x));
	else
		check.y = game->p.pos.y + step.y * (game->p.pos.x - (int)game->p.pos.x);
	while ((unsigned)check.y < game->map.size.y && \
			(unsigned)check.x < game->map.size.x && \
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
		check.x = game->p.pos.x + step.x * \
								(1 - (game->p.pos.y - (int)game->p.pos.y));
	else
		check.x = game->p.pos.x + step.x * (game->p.pos.y - (int)game->p.pos.y);
	while ((unsigned)check.y < game->map.size.y && \
			(unsigned)check.x < game->map.size.x && \
			game->map.grid[(unsigned)check.y][(unsigned)check.x] != '1')
		check = (t_fpoint){check.x + step.x, check.y + step.y};
	check.y += (step.y < 0);
	return (check);
}

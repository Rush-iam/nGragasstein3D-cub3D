/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:45 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 14:22:18 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void		ray_cast(t_game *game)
{
	int		ray;
	float	angle;

	ray = 0;
	while (ray < (int)game->img.size.x)
	{
		angle = game->p.angle + game->angles[ray];
		game->column[ray] = ray_intersect(game, tanf(angle),
			(t_point){1 - (angle < -M_PI_2_F || angle >= M_PI_2_F) * 2,
			1 - (sinf(angle) < 0.0f) * 2});
		ray++;
	}
}

struct s_column	ray_intersect(t_game *game, float tan_cur_angle, t_point step)
{
	const t_ray	x1 = ray_intersect_x(game, step.x, step.x * tan_cur_angle);
	const t_ray	y1 = ray_intersect_y(game, step.y / tan_cur_angle, step.y);
	const float	distance_x = game->p.vect.x * (x1.pos.x - game->p.pos.x) +
							game->p.vect.y * (x1.pos.y - game->p.pos.y);
	const float	distance_y = game->p.vect.x * (y1.pos.x - game->p.pos.x) +
							game->p.vect.y * (y1.pos.y - game->p.pos.y);

	if (distance_x < distance_y)
		return ((struct s_column){distance_x, x1.pos, x1.cell,
				"EW"[x1.cell.x < (int)game->p.pos.x], 0, 0, 0});
	else
		return ((struct s_column){distance_y, y1.pos, y1.cell,
				"SN"[y1.cell.y < (int)game->p.pos.y], 0, 0, 0});
}

static inline bool	ray_intersect_door(t_game *g, float x, float st_x, t_point pt)
{
	const float	check = x + st_x / 2.0f - (int)x;

	return (door_find(g, pt)->part_opened <= check && check < 1.0f);
}

static inline bool	ray_intersect_secret(t_game *g, float *x, float *st_x, t_point pt)
{
	const float	part_opened = door_find(g, pt)->part_opened;
	const float	check = *x + *st_x * part_opened - (int)*x;

	if (0.0f <= check && check < 1.0f)
	{
		*x = check + (int)*x;
		*st_x = part_opened;
		return (true);
	}
	return (false);
}

t_ray	ray_intersect_x(t_game *g, int stx, float sty)
{
	int		x;
	float	y;
	char	chr;

	x = (int)g->p.pos.x + stx;
	y = g->p.pos.y + sty * ((stx > 0) - stx * (g->p.pos.x - (int)g->p.pos.x));
	while ((unsigned)y < g->map.size.y && (unsigned)x < g->map.size.x)
	{
		chr = g->map.grid[(unsigned)y][x];
		if (chr == '.' || chr == CHAR_SOLID_MAP)
			;
		else if (chr_is_wall(chr))
			break ;
		else if ((chr == CHAR_DOORS_V[0] || chr == CHAR_DOORS_V[1]) && \
				ray_intersect_door(g, y, sty, (t_point){x, y}))
			return ((t_ray){(t_fpoint){x + stx / 2.0f + (stx < 0),
				y + sty / 2.0f}, (t_point){x, y}});
		else if (chr == *CHAR_SECRET && \
			ray_intersect_secret(g, &y, &sty, (t_point){x, y}))
			return ((t_ray)
				{(t_fpoint){x + stx * sty + (stx < 0), y}, (t_point){x, y}});
		x += stx;
		y += sty;
	}
	return ((t_ray){(t_fpoint){x + (stx < 0), y}, (t_point){x, y}});
}

t_ray	ray_intersect_y(t_game *g, float stx, int sty)
{
	float	x;
	int		y;
	char	chr;

	y = (int)g->p.pos.y + sty;
	x = g->p.pos.x + stx * ((sty > 0) - sty * (g->p.pos.y - (int)g->p.pos.y));
	while ((unsigned)y < g->map.size.y && (unsigned)x < g->map.size.x)
	{
		chr = g->map.grid[y][(unsigned)x];
		if (chr == '.' || chr == CHAR_SOLID_MAP)
			;
		else if (chr_is_wall(chr))
			break ;
		else if ((chr == CHAR_DOORS_H[0] || chr == CHAR_DOORS_H[1]) && \
			ray_intersect_door(g, x, stx, (t_point){x, y}))
			return ((t_ray){(t_fpoint){x + stx / 2.0f,
				y + sty / 2.0f + (sty < 0)}, (t_point){x, y}});
		else if (chr == *CHAR_SECRET && \
			ray_intersect_secret(g, &x, &stx, (t_point){x, y}))
			return ((t_ray)
				{(t_fpoint){x, y + sty * stx + (sty < 0)}, (t_point){x, y}});
		x += stx;
		y += sty;
	}
	return ((t_ray){(t_fpoint){x, y + (sty < 0)}, (t_point){x, y}});
}

float	ray_intersect_distance(t_game *game, float angle)
{
	t_ray		x1;
	t_ray		y1;
	t_fpoint	distance;
	const float	tan_cur_angle = tanf(angle);

	x1 = (angle < -M_PI_2_F || angle >= M_PI_2_F) ?
		 ray_intersect_x(game, -1, -tan_cur_angle) :
		 ray_intersect_x(game, 1, tan_cur_angle);
	y1 = (angle >= 0) ?
		 ray_intersect_y(game, 1 / tan_cur_angle, 1) :
		 ray_intersect_y(game, -1 / tan_cur_angle, -1);
	distance.x = hypotf(x1.pos.x - game->p.pos.x, x1.pos.y - game->p.pos.y);
	distance.y = hypotf(y1.pos.x - game->p.pos.x, y1.pos.y - game->p.pos.y);
	return (fminf(distance.x, distance.y));
}

// DDA intersection:
//
//bool	ray_intersect_door_x(t_game *g, t_point step, t_fpoint dist_step, t_ray *ray)
//{
//	t_fpoint	check;
//
//	check.x = ray->cell.x + step.x / 2.0f;
////	check.y = ray->cell.y + (dist_step.x - fabsf(g->p.vect.x)) / fabsf(g->p.vect.y) / 2.0f;
//	if (!(0.0f < check.y && check.y < door_find(g, ray->cell)->part_opened))
//	{
//		ray->pos.x += dist_step.x / 2.0f;
//		return (true);
//	}
//	else
//		return (false);
//}
//
//t_ray	ray_intersect_get(t_game *g, t_point step, t_fpoint dist_step, t_ray ray)
//{
//	while ((unsigned)ray.cell.x < g->map.size.x &&
//			(unsigned)ray.cell.y < g->map.size.y)
//	{
//		if (ray.pos.x < ray.pos.y)
//		{
//			ray.cell.x += step.x;
//			if (chr_is_wall(g->map.grid[ray.cell.y][ray.cell.x]))
//				break ;
////			else if (ft_memchr(CHAR_DOORS, chr, sizeof(CHAR_DOORS)) &&
////					ray_intersect_door_x(g, step, dist_step, &ray))
////				return (ray);
//			ray.pos.x += dist_step.x;
//		}
//		else
//		{
//			ray.cell.y += step.y;
//			if (chr_is_wall(g->map.grid[ray.cell.y][ray.cell.x]))
//				break ;
//			ray.pos.y += dist_step.y;
//		}
//	}
//	if ((unsigned)ray.cell.x >= g->map.size.x ||
//		(unsigned)ray.cell.y >= g->map.size.y)
//		return ((t_ray){T_FPT_NULL, (t_point){-1, 0}});
//	return (ray);
//}
//
//
//struct s_column	ray_intersect_dda(t_game *g, float tan_angle, t_point negative)
//{
//	const t_point	step = {!negative.x - negative.x, !negative.y - negative.y};
//	t_fpoint		dist_step;
//	t_ray			ray;
//
//	dist_step.x = fabsf(g->p.vect.x + g->p.vect.y * tan_angle);
//	dist_step.y = fabsf(g->p.vect.y + g->p.vect.x / tan_angle);
//	ray.pos.x = dist_step.x * (negative.x ? (g->p.pos.x - (int)g->p.pos.x) :
//								(1.0f - (g->p.pos.x - (int)g->p.pos.x)));
//	ray.pos.y = dist_step.y * (negative.y ? (g->p.pos.y - (int)g->p.pos.y) :
//								(1.0f - (g->p.pos.y - (int)g->p.pos.y)));
//	ray.cell = (t_point){g->p.pos.x, g->p.pos.y};
//	ray = ray_intersect_get(g, step, dist_step, ray);
//	if (ray.cell.x == -1)
//		return ((struct s_column){0, T_FPT_NULL, ray.cell, 0, 0, 0, 0});
//	if (ray.pos.x < ray.pos.y)
//		return ((struct s_column){ray.pos.x, (t_fpoint){ray.cell.x + negative.x,
//			g->p.pos.y + (ray.cell.x + negative.x - g->p.pos.x) * tan_angle},
//							ray.cell, "EW"[negative.x], 0, 0, 0});
//	else
//		return ((struct s_column){ray.pos.y,
//	(t_fpoint){g->p.pos.x + (ray.cell.y + negative.y - g->p.pos.y) / tan_angle,
//							ray.cell.y + negative.y},
//							ray.cell, "SN"[negative.y], 0, 0, 0});
//}

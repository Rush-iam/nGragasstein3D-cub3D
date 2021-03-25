/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:45 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/25 23:18:15 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_wall_texture_set(t_game *game, struct s_column *col, t_point cell)
{
	const char	chr = game->map.grid[cell.y][cell.x];

	if ((chr == '>' && (col->dir == 'W' || col->dir == 'E')) ||
		(chr == 'v' && (col->dir == 'N' || col->dir == 'S')))
	{
		col->texture_id = TEXTURE_DOOR;
		col->texture_pos =
			fmaxf(0.0f, col->texture_pos - door_find(game, cell)->part_opened);
	}
	else if ((chr == 'v' && (col->dir == 'W' || col->dir == 'E')) ||
		(chr == '>' && (col->dir == 'N' || col->dir == 'S')))
		col->texture_id = TEXTURE_DOOR_W;
	else if ((col->dir == 'N' && game->map.grid[cell.y + 1][cell.x] == '>') ||
		(col->dir == 'S' && game->map.grid[cell.y - 1][cell.x] == '>') ||
		(col->dir == 'W' && game->map.grid[cell.y][cell.x + 1] == 'v') ||
		(col->dir == 'E' && game->map.grid[cell.y][cell.x - 1] == 'v'))
		col->texture_id = TEXTURE_DOOR_W;
	else
		col->texture_id = chr - '0';
	if (((col->dir == 'W' || col->dir == 'E') &&
										col->texture_id != TEXTURE_DOOR_W) ||
		(col->texture_id == TEXTURE_DOOR_W &&
										(col->dir == 'N' || col->dir == 'S')))
		col->texture_id += sizeof(game->texture) / sizeof(*game->texture) / 2;
	if (chr != '>' && chr != 'v' && (col->dir == 'S' || col->dir == 'W'))
		col->texture_pos = 1.0f - col->texture_pos;
}

void		ray_cast(t_game *game)
{
	unsigned	ray;
	float		angle;

	ray = 0;
	while (ray < game->img.size.x)
	{
		angle = game->p.angle + atanf(game->col_step * (ray - game->col_center));
		if (angle < 0.0f)
			angle += PI2_F;
		else if (angle > PI2_F)
			angle -= PI2_F;
		game->column[ray] = ray_intersect(game, angle);
		if (game->column[ray].cell.x < (int)game->map.size.x &&
			game->column[ray].cell.y < (int)game->map.size.y)
			draw_wall_texture_set(game, &game->column[ray], game->column[ray].cell);
		ray++;
	}
}

struct s_column	ray_intersect(t_game *game, float cur_angle)
{
	t_fpoint	x1;
	t_fpoint	y1;
	t_fpoint	distance;
	const float	tan_cur_angle = tanf(cur_angle);

	x1 = (cur_angle <= M_PI_2_F || cur_angle > 3.0f * M_PI_2_F) ?
		 ray_intersect_x(game, game->p.pos, (t_fpoint){1, tan_cur_angle}) :
		 ray_intersect_x(game, game->p.pos, (t_fpoint){-1, -tan_cur_angle});
	y1 = (cur_angle <= M_PI_F) ?
		 ray_intersect_y(game, game->p.pos, (t_fpoint){1 / tan_cur_angle, 1}) :
		 ray_intersect_y(game, game->p.pos, (t_fpoint){-1 / tan_cur_angle, -1});
	distance.x = game->p.vector.x * (x1.x - game->p.pos.x) +
				 game->p.vector.y * (x1.y - game->p.pos.y);
	distance.y = game->p.vector.x * (y1.x - game->p.pos.x) +
				 game->p.vector.y * (y1.y - game->p.pos.y);
	if (distance.x < distance.y)
		return ((struct s_column){distance.x, game->col_scale / distance.x,
	(t_point){x1.x, x1.y}, -1U, x1.y - (int)x1.y, "EW"[x1.x < game->p.pos.x]});
	else
		return ((struct s_column){distance.y, game->col_scale / distance.y,
	(t_point){y1.x, y1.y}, -1U, y1.x - (int)y1.x, "SN"[y1.y < game->p.pos.y]});
}

t_fpoint	ray_intersect_x_door(t_game *game, t_fpoint step, t_fpoint check)
{
	t_fpoint	check_door;
	float		position;

	check_door = (t_fpoint){check.x + step.x / 2, check.y + step.y / 2};
	position = check_door.y - (unsigned)check.y;
	if (0 < position &&
		position < door_find(game, (t_point){check.x, check.y})->part_opened)
		return (ray_intersect_x(game, (t_fpoint){
				check_door.x + (step.x < 0), check_door.y}, step));
	return ((t_fpoint){check_door.x + (step.x < 0), check_door.y});
}

t_fpoint	ray_intersect_y_door(t_game *game, t_fpoint step, t_fpoint check)
{
	t_fpoint	check_door;
	float		position;

	check_door = (t_fpoint){check.x + step.x / 2, check.y + step.y / 2};
	position = check_door.x - (unsigned)check.x;
	if (0 < position &&
		position < door_find(game, (t_point){check.x, check.y})->part_opened)
		return (ray_intersect_y(game, (t_fpoint){
				check_door.x, check_door.y + (step.y < 0)}, step));
	return ((t_fpoint){check_door.x, check_door.y + (step.y < 0)});
}

t_fpoint	ray_intersect_x(t_game *game, t_fpoint from, t_fpoint step)
{
	t_fpoint	check;

	check.x = (int)from.x + (step.x > 0) - (step.x < 0);
	if (step.x > 0)
		check.y = from.y + step.y * (1 - (from.x - (int)from.x));
	else
		check.y = from.y + step.y * (from.x - (int)from.x);
	while ((unsigned)check.y < game->map.size.y &&
			(unsigned)check.x < game->map.size.x)
	{
		if (ft_isdigit(game->map.grid[(unsigned)check.y][(unsigned)check.x]))
			break ;
		else if (ft_memchr(CHAR_DOORS, game->map.grid[(unsigned)check.y]
									[(unsigned)check.x], sizeof(CHAR_DOORS)))
			return (ray_intersect_x_door(game, step, check));
		check = (t_fpoint){check.x + step.x, check.y + step.y};
	}
	if (step.x < 0)
		check.x += (step.x < 0) - FLOAT_FIX;
	return (check);
}

t_fpoint	ray_intersect_y(t_game *game, t_fpoint from, t_fpoint step)
{
	t_fpoint	check;

	check.y = (int)from.y + (step.y > 0) - (step.y < 0);
	if (step.y > 0)
		check.x = from.x + step.x * (1 - (from.y - (int)from.y));
	else
		check.x = from.x + step.x * (from.y - (int)from.y);
	while ((unsigned)check.y < game->map.size.y &&
			(unsigned)check.x < game->map.size.x)
	{
		if (ft_isdigit(game->map.grid[(unsigned)check.y][(unsigned)check.x]))
			break ;
		else if (ft_memchr(CHAR_DOORS, game->map.grid[(unsigned)check.y]
									[(unsigned)check.x], sizeof(CHAR_DOORS)))
			return (ray_intersect_y_door(game, step, check));
		check = (t_fpoint){check.x + step.x, check.y + step.y};
	}
	if (step.y < 0)
		check.y += (step.y < 0) - FLOAT_FIX;
	return (check);
}

float	ray_intersect_distance(t_game *game, float cur_angle)
{
	t_fpoint	x1;
	t_fpoint	y1;
	t_fpoint	distance;
	const float	tan_cur_angle = tanf(cur_angle);

	x1 = (cur_angle < -M_PI_2_F || cur_angle >= M_PI_2_F) ?
		 ray_intersect_x(game, game->p.pos, (t_fpoint){-1, -tan_cur_angle}) :
		 ray_intersect_x(game, game->p.pos, (t_fpoint){1, tan_cur_angle});
	y1 = (cur_angle >= 0) ?
		 ray_intersect_y(game, game->p.pos, (t_fpoint){1 / tan_cur_angle, 1}) :
		 ray_intersect_y(game, game->p.pos, (t_fpoint){-1 / tan_cur_angle, -1});
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

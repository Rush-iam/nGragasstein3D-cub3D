/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_borders_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 14:28:00 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 14:28:00 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	player_control_borders(t_game *g)
{
	const t_point	plus = {g->p.pos.x + PL_RADIUS, g->p.pos.y + PL_RADIUS};
	const t_point	minus = {g->p.pos.x - PL_RADIUS, g->p.pos.y - PL_RADIUS};
	char			check;

	check = g->map.grid[(int)g->p.pos.y][minus.x];
	if (check != '.' && (chr_is_wall(check) || check == CHAR_SOLID_MAP || \
	check == *CHAR_SECRET || (ft_memchr(CHAR_DOORS, check, sizeof(CHAR_DOORS)) \
	&& door_find(g, (t_point){minus.x, (int)g->p.pos.y})->part_opened < 0.5f)))
		g->p.pos.x = minus.x + 1 + PL_RADIUS + FLOAT_FIX;
	check = g->map.grid[(int)g->p.pos.y][plus.x];
	if (check != '.' && (chr_is_wall(check) || check == CHAR_SOLID_MAP || \
	check == *CHAR_SECRET || (ft_memchr(CHAR_DOORS, check, sizeof(CHAR_DOORS)) \
	&& door_find(g, (t_point){plus.x, (int)g->p.pos.y})->part_opened < 0.5f)))
		g->p.pos.x = plus.x - PL_RADIUS - FLOAT_FIX;
	check = g->map.grid[minus.y][(int)g->p.pos.x];
	if (check != '.' && (chr_is_wall(check) || check == CHAR_SOLID_MAP || \
	check == *CHAR_SECRET || (ft_memchr(CHAR_DOORS, check, sizeof(CHAR_DOORS)) \
	&& door_find(g, (t_point){(int)g->p.pos.x, minus.y})->part_opened < 0.5f)))
		g->p.pos.y = minus.y + 1 + PL_RADIUS + FLOAT_FIX;
	check = g->map.grid[plus.y][(int)g->p.pos.x];
	if (check != '.' && (chr_is_wall(check) || check == CHAR_SOLID_MAP || \
	check == *CHAR_SECRET || (ft_memchr(CHAR_DOORS, check, sizeof(CHAR_DOORS)) \
	&& door_find(g, (t_point){(int)g->p.pos.x, plus.y})->part_opened < 0.5f)))
		g->p.pos.y = plus.y - PL_RADIUS - FLOAT_FIX;
}

void	player_control_borders_diag(t_game *g)
{
	const t_point	plus = {g->p.pos.x + PL_RADIUS, g->p.pos.y + PL_RADIUS};
	const t_point	minus = {g->p.pos.x - PL_RADIUS, g->p.pos.y - PL_RADIUS};

	if (chr_is_wall(g->map.grid[minus.y][minus.x]))
	{
		if (g->p.pos.x - (int)g->p.pos.x > g->p.pos.y - (int)g->p.pos.y)
			g->p.pos.x = minus.x + 1 + PL_RADIUS;
		else
			g->p.pos.y = minus.y + 1 + PL_RADIUS;
	}
	if (chr_is_wall(g->map.grid[plus.y][plus.x]))
	{
		if (g->p.pos.x - (int)g->p.pos.x < g->p.pos.y - (int)g->p.pos.y)
			g->p.pos.x = plus.x - PL_RADIUS;
		else
			g->p.pos.y = plus.y - PL_RADIUS;
	}
	player_control_borders_diag_2(g, plus, minus);
}

void	player_control_borders_diag_2(t_game *g, t_point plus, t_point minus)
{
	if (chr_is_wall(g->map.grid[minus.y][plus.x]))
	{
		if (1.f - (g->p.pos.x - (int)g->p.pos.x) > g->p.pos.y - (int)g->p.pos.y)
			g->p.pos.x = plus.x - PL_RADIUS;
		else
			g->p.pos.y = minus.y + 1 + PL_RADIUS;
	}
	if (chr_is_wall(g->map.grid[plus.y][minus.x]))
	{
		if (g->p.pos.x - (int)g->p.pos.x > 1.f - (g->p.pos.y - (int)g->p.pos.y))
			g->p.pos.x = minus.x + 1 + PL_RADIUS;
		else
			g->p.pos.y = plus.y - PL_RADIUS;
	}
}

void	player_control_borders_enemies(t_game *game)
{
	t_list		*cur_list;
	t_object	*obj;
	t_fpoint	diff;

	cur_list = game->objects;
	while (cur_list)
	{
		obj = (t_object *)cur_list->content;
		diff.x = obj->pos.x - game->p.pos.x;
		diff.y = obj->pos.y - game->p.pos.y;
		obj->distance_real = hypotf(diff.x, diff.y);
		if (obj->distance_real < PL_RADIUS * 2 && \
			obj->type == T_ENEMY && obj->e->state != ST_DEATH)
		{
			obj->atan_diff = atan2f(diff.y, diff.x);
			obj->distance = game->p.vect.x * diff.x + game->p.vect.y * diff.y;
			game->p.pos.x = obj->pos.x - PL_RADIUS * 2 * cosf(obj->atan_diff);
			game->p.pos.y = obj->pos.y - PL_RADIUS * 2 * sinf(obj->atan_diff);
		}
		cur_list = cur_list->next;
	}
}

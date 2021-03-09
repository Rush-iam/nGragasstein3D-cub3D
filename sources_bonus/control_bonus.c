/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:31:39 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/09 18:36:45 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	player_control(t_game *game)
{
	player_control_rotate(game);
	player_control_move(game);
	player_control_extra(game);
	player_control_borders(game);
}

void	player_control_rotate(t_game *game)
{
	mlx_mouse_get_pos(game->win, &game->key.mpos.x, &game->key.mpos.y);
	mlx_mouse_move(game->win, game->win_center.x, game->win_center.y);
	game->key.mdir.x = game->key.mdir.x / 2 + game->key.mpos.x -
					   game->win_center.x;
	game->p.angle += game->key.mdir.x / MOUSE_SPEED;
	if (game->key.k[TURN_LEFT])
		game->p.angle -= PL_SPEED / 3;
	if (game->key.k[TURN_RIGHT])
		game->p.angle += PL_SPEED / 3;
	if (game->p.angle > PI2)
		game->p.angle -= PI2;
	else if (game->p.angle < 0)
		game->p.angle += PI2;
	__sincos(game->p.angle, &game->p.vector.y, &game->p.vector.x);
}

void	player_control_move(t_game *game)
{
	if (game->key.k[MOVE_FORWARD])
		game->p.pos = (t_fpoint){
		game->p.pos.x + (game->key.k[RUN] + 1) * PL_SPEED * game->p.vector.x,
		game->p.pos.y + (game->key.k[RUN] + 1) * PL_SPEED * game->p.vector.y};
	if (game->key.k[MOVE_BACK])
		game->p.pos = (t_fpoint){
	game->p.pos.x - (game->key.k[RUN] + 1) * PL_SPEED / 1.5 * game->p.vector.x,
	game->p.pos.y - (game->key.k[RUN] + 1) * PL_SPEED / 1.5 * game->p.vector.y};
	if (game->key.k[MOVE_LEFT])
		game->p.pos = (t_fpoint){
		game->p.pos.x + (game->key.k[RUN] + 1) * PL_SPEED * game->p.vector.y,
		game->p.pos.y - (game->key.k[RUN] + 1) * PL_SPEED * game->p.vector.x};
	if (game->key.k[MOVE_RIGHT])
		game->p.pos = (t_fpoint){
		game->p.pos.x - (game->key.k[RUN] + 1) * PL_SPEED * game->p.vector.y,
		game->p.pos.y + (game->key.k[RUN] + 1) * PL_SPEED * game->p.vector.x};
}

void	player_control_toggler(t_game *game, int key_code)
{
	if (key_code == MAP_TOGGLE && game->key.k[key_code] == false)
		game->map.show = !game->map.show;
}

void	player_control_extra(t_game *game)
{
	if (game->key.k[FOV_WIDE])
		set_fov(game, game->fov * FOV_ZOOMSPEED, false);
	if (game->key.k[FOV_TELE])
		set_fov(game, game->fov / FOV_ZOOMSPEED, false);
	if (game->key.k[FOV_RESET])
		set_fov(game, 0, true);
}


void	player_control_borders(t_game *g)
{
	const t_point	plus = {g->p.pos.x + PL_RADIUS, g->p.pos.y + PL_RADIUS};
	const t_point	minus = {g->p.pos.x - PL_RADIUS, g->p.pos.y - PL_RADIUS};

	if (g->p.pos.x < PL_RADIUS)
		g->p.pos.x = PL_RADIUS + MAP_CELL_FIX;
	if (g->p.pos.y < PL_RADIUS)
		g->p.pos.y = PL_RADIUS + MAP_CELL_FIX;
	if (g->p.pos.x + PL_RADIUS >= g->map.size.x)
		g->p.pos.x = g->map.size.x - PL_RADIUS - MAP_CELL_FIX;
	if (g->p.pos.y + PL_RADIUS >= g->map.size.y)
		g->p.pos.y = g->map.size.y - PL_RADIUS - MAP_CELL_FIX;
	if (ft_isdigit(g->map.grid[(int)g->p.pos.y][minus.x]) || ft_memchr(
		CHAR_SOLID, g->map.grid[(int)g->p.pos.y][minus.x], sizeof(CHAR_SOLID)))
		g->p.pos.x = minus.x + 1 + PL_RADIUS + MAP_CELL_FIX;
	if (ft_isdigit(g->map.grid[(int)g->p.pos.y][plus.x]) || ft_memchr(
		CHAR_SOLID, g->map.grid[(int)g->p.pos.y][plus.x], sizeof(CHAR_SOLID)))
		g->p.pos.x = plus.x - PL_RADIUS - MAP_CELL_FIX;
	if (ft_isdigit(g->map.grid[minus.y][(int)g->p.pos.x]) || ft_memchr(
		CHAR_SOLID, g->map.grid[minus.y][(int)g->p.pos.x], sizeof(CHAR_SOLID)))
		g->p.pos.y = minus.y + 1 + PL_RADIUS + MAP_CELL_FIX;
	if (ft_isdigit(g->map.grid[plus.y][(int)g->p.pos.x]) || ft_memchr(
		CHAR_SOLID, g->map.grid[plus.y][(int)g->p.pos.x], sizeof(CHAR_SOLID)))
		g->p.pos.y = plus.y - PL_RADIUS - MAP_CELL_FIX;
	player_control_borders_diag(g);
}

void	player_control_borders_diag(t_game *g)
{
	const t_point	plus = {g->p.pos.x + PL_RADIUS, g->p.pos.y + PL_RADIUS};
	const t_point	minus = {g->p.pos.x - PL_RADIUS, g->p.pos.y - PL_RADIUS};

	if (ft_isdigit(g->map.grid[minus.y][minus.x]) || ft_memchr(
			CHAR_SOLID, g->map.grid[minus.y][minus.x], sizeof(CHAR_SOLID)))
		(g->p.pos.x - (int)g->p.pos.x > g->p.pos.y - (int)g->p.pos.y) ?
		(g->p.pos.x = minus.x + 1 + PL_RADIUS) :
		(g->p.pos.y = minus.y + 1 + PL_RADIUS);
	if (ft_isdigit(g->map.grid[plus.y][plus.x]) || ft_memchr(
			CHAR_SOLID, g->map.grid[plus.y][plus.x], sizeof(CHAR_SOLID)))
		(g->p.pos.x - (int)g->p.pos.x < g->p.pos.y - (int)g->p.pos.y) ?
		(g->p.pos.x = plus.x - PL_RADIUS) :
		(g->p.pos.y = plus.y - PL_RADIUS);
	if (ft_isdigit(g->map.grid[minus.y][plus.x]) || ft_memchr(
			CHAR_SOLID, g->map.grid[minus.y][plus.x], sizeof(CHAR_SOLID)))
		(1. - (g->p.pos.x - (int)g->p.pos.x) > g->p.pos.y - (int)g->p.pos.y) ?
		(g->p.pos.x = plus.x - PL_RADIUS) :
		(g->p.pos.y = minus.y + 1 + PL_RADIUS);
	if (ft_isdigit(g->map.grid[plus.y][minus.x]) || ft_memchr(
			CHAR_SOLID, g->map.grid[plus.y][minus.x], sizeof(CHAR_SOLID)))
		(g->p.pos.x - (int)g->p.pos.x > 1. - (g->p.pos.y - (int)g->p.pos.y)) ?
		(g->p.pos.x = minus.x + 1 + PL_RADIUS) :
		(g->p.pos.y = plus.y - PL_RADIUS);
}

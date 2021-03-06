/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:31:39 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/06 23:46:08 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	player_control(t_game *game)
{
	player_control_move(game);
	player_control_extra(game);
	player_control_borders(game);
	__sincos(game->p.angle, &game->p.cossin.y, &game->p.cossin.x);
}

void	player_control_move(t_game *game)
{
	mlx_mouse_get_pos(game->win, &game->key.mpos.x, &game->key.mpos.y);
	mlx_mouse_move(game->win, game->win_center.x, game->win_center.y);
	game->key.mdir.x = game->key.mdir.x / 2 + game->key.mpos.x -
															game->win_center.x;
	game->p.angle += game->key.mdir.x / MOUSE_SPEED;
	if (game->key.k[TURN_LEFT])
		game->p.angle -= PL_SPEED / 2;
	if (game->key.k[TURN_RIGHT])
		game->p.angle += PL_SPEED / 2;
	if (game->p.angle > PI2)
		game->p.angle -= PI2;
	if (game->p.angle < 0)
		game->p.angle += PI2;
	if (game->key.k[MOVE_FORWARD])
		game->p.pos = (t_fpoint){game->p.pos.x + PL_SPEED * game->p.cossin.x,
								 game->p.pos.y + PL_SPEED * game->p.cossin.y};
	if (game->key.k[MOVE_BACK])
		game->p.pos = (t_fpoint){game->p.pos.x - PL_SPEED * game->p.cossin.x,
								 game->p.pos.y - PL_SPEED * game->p.cossin.y};
	if (game->key.k[MOVE_LEFT])
		game->p.pos = (t_fpoint){game->p.pos.x + PL_SPEED * game->p.cossin.y,
								 game->p.pos.y - PL_SPEED * game->p.cossin.x};
	if (game->key.k[MOVE_RIGHT])
		game->p.pos = (t_fpoint){game->p.pos.x - PL_SPEED * game->p.cossin.y,
								 game->p.pos.y + PL_SPEED * game->p.cossin.x};
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
	if (g->p.pos.x < BUMP_RADIUS)
		g->p.pos.x = BUMP_RADIUS;
	if (g->p.pos.y < BUMP_RADIUS)
		g->p.pos.y = BUMP_RADIUS;
	if (g->p.pos.x + BUMP_RADIUS >= g->map.size.x)
		g->p.pos.x = g->map.size.x - BUMP_RADIUS;
	if (g->p.pos.y + BUMP_RADIUS >= g->map.size.y)
		g->p.pos.y = g->map.size.y - BUMP_RADIUS;
	if (g->map.grid[(int)(g->p.pos.y - BUMP_RADIUS)][(int)g->p.pos.x] == '1')
		g->p.pos.y = (int)(g->p.pos.y - BUMP_RADIUS) + 1 + BUMP_RADIUS;
	if (g->map.grid[(int)(g->p.pos.y + BUMP_RADIUS)][(int)g->p.pos.x] == '1')
		g->p.pos.y = (int)(g->p.pos.y + BUMP_RADIUS) - BUMP_RADIUS;
	if (g->map.grid[(int)g->p.pos.y][(int)(g->p.pos.x - BUMP_RADIUS)] == '1')
		g->p.pos.x = (int)(g->p.pos.x - BUMP_RADIUS) + 1 + BUMP_RADIUS;
	if (g->map.grid[(int)g->p.pos.y][(int)(g->p.pos.x + BUMP_RADIUS)] == '1')
		g->p.pos.x = (int)(g->p.pos.x + BUMP_RADIUS) - BUMP_RADIUS;

//	if (g->map.grid[(int)(g->p.pos.y - BUMP_RADIUS)][(int)(g->p.pos.x - BUMP_RADIUS)] == '1')
//	{
//		g->p.pos.y = (int)(g->p.pos.y - BUMP_RADIUS) + 1 + BUMP_RADIUS;
//		g->p.pos.x = (int)(g->p.pos.x - BUMP_RADIUS) + 1 + BUMP_RADIUS;
//	}
//	if (g->map.grid[(int)(g->p.pos.y + BUMP_RADIUS)][(int)g->p.pos.x] == '1')
//		g->p.pos.y = (int)(g->p.pos.y + BUMP_RADIUS) - BUMP_RADIUS;
//	if (g->map.grid[(int)g->p.pos.y][(int)(g->p.pos.x - BUMP_RADIUS)] == '1')
//		g->p.pos.x = (int)(g->p.pos.x - BUMP_RADIUS) + 1 + BUMP_RADIUS;
//	if (g->map.grid[(int)g->p.pos.y][(int)(g->p.pos.x + BUMP_RADIUS)] == '1')
//		g->p.pos.x = (int)(g->p.pos.x + BUMP_RADIUS) - BUMP_RADIUS;
}

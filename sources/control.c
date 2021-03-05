/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 20:55:59 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/05 19:04:15 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	player_control(t_game *game)
{
//	t_point	mouse_pos;
//
//	mlx_mouse_get_pos(game->win, &mouse_pos.x, &mouse_pos.y);
//	mlx_mouse_move(game->win, game->img.size.x / 2, game->img.size.y / 2);
//	game->key.mdir.x = mouse_pos.x - game->img.size.x / 2;
//	game->p.angle += game->key.mdir.x / 1000.;
	if (game->key.k[TURN_LEFT])
		game->p.angle -= PL_SPEED / 2;
	if (game->key.k[TURN_RIGHT])
		game->p.angle += PL_SPEED / 2;
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
	if (game->key.k[FOV_WIDE])
		set_fov(game, game->fov * 1.03);
	else if (game->key.k[FOV_TELE])
		set_fov(game, game->fov / 1.03);
	player_control_borders(game);
	__sincos(game->p.angle, &game->p.cossin.y, &game->p.cossin.x);
}

void	player_control_borders(t_game *game)
{
	if (game->p.angle > PI2)
		game->p.angle -= PI2;
	if (game->p.angle < 0)
		game->p.angle += PI2;
	if (game->p.pos.x < 0)
		game->p.pos.x = 0.0001;
	if (game->p.pos.y < 0)
		game->p.pos.y = 0.0001;
	if (game->p.pos.x >= game->map.size.x)
		game->p.pos.x = game->map.size.x - 0.0001;
	if (game->p.pos.y >= game->map.size.y)
		game->p.pos.y = game->map.size.y - 0.0001;
}

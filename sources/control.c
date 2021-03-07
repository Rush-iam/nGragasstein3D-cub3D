/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 20:55:59 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/06 17:52:14 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	player_control(t_game *game)
{
	if (game->key[TURN_LEFT])
		game->p.angle -= PL_SPEED / 2;
	if (game->key[TURN_RIGHT])
		game->p.angle += PL_SPEED / 2;
	if (game->key[MOVE_FORWARD])
		game->p.pos = (t_fpoint){game->p.pos.x + PL_SPEED * game->p.vector.x,
								game->p.pos.y + PL_SPEED * game->p.vector.y};
	if (game->key[MOVE_BACK])
		game->p.pos = (t_fpoint){game->p.pos.x - PL_SPEED * game->p.vector.x,
								game->p.pos.y - PL_SPEED * game->p.vector.y};
	if (game->key[MOVE_LEFT])
		game->p.pos = (t_fpoint){game->p.pos.x + PL_SPEED * game->p.vector.y,
								game->p.pos.y - PL_SPEED * game->p.vector.x};
	if (game->key[MOVE_RIGHT])
		game->p.pos = (t_fpoint){game->p.pos.x - PL_SPEED * game->p.vector.y,
								game->p.pos.y + PL_SPEED * game->p.vector.x};
	if (game->key[FOV_WIDE])
		set_fov(game, game->fov * 1.03, false);
	if (game->key[FOV_TELE])
		set_fov(game, game->fov / 1.03, false);
	if (game->key[FOV_RESET])
		set_fov(game, 0, true);
	player_control_borders(game);
	__sincos(game->p.angle, &game->p.vector.y, &game->p.vector.x);
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

int		hook_key_press(int key_code, t_game *game)
{
	if (key_code >= (int)sizeof(game->key))
		return (1);
	if (key_code == KEY_ESCAPE)
		terminate(game, EXIT_SUCCESS, NULL);
	game->key[key_code] = true;
	return (0);
}

int		hook_key_release(int key_code, t_game *game)
{
	if (key_code >= (int)sizeof(game->key))
		return (1);
	game->key[key_code] = false;
	return (0);
}

int		hook_exit(t_game *game)
{
	return (terminate(game, 0, NULL));
}

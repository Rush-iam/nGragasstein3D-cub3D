/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 20:55:59 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/02 19:55:11 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline bool	key_pressed(t_game *game, int key)
{
	return (game->key[(short)key + KEYCODE_OFFSET]);
}

void	player_control(t_game *game)
{
	if (key_pressed(game, K_TURN_LEFT))
		game->p.angle -= PL_SPEED / 2;
	if (key_pressed(game, K_TURN_RIGHT))
		game->p.angle += PL_SPEED / 2;
	if (key_pressed(game, K_MOVE_FORWARD))
		game->p.pos = (t_fpoint){game->p.pos.x + PL_SPEED * game->p.vect.x, \
								game->p.pos.y + PL_SPEED * game->p.vect.y};
	if (key_pressed(game, K_MOVE_BACK))
		game->p.pos = (t_fpoint){game->p.pos.x - PL_SPEED * game->p.vect.x, \
								game->p.pos.y - PL_SPEED * game->p.vect.y};
	if (key_pressed(game, K_MOVE_LEFT))
		game->p.pos = (t_fpoint){game->p.pos.x + PL_SPEED * game->p.vect.y, \
								game->p.pos.y - PL_SPEED * game->p.vect.x};
	if (key_pressed(game, K_MOVE_RIGHT))
		game->p.pos = (t_fpoint){game->p.pos.x - PL_SPEED * game->p.vect.y, \
								game->p.pos.y + PL_SPEED * game->p.vect.x};
	if (key_pressed(game, K_FOV_WIDE))
		player_set_fov(game, game->fov * 1.03, false);
	if (key_pressed(game, K_FOV_TELE))
		player_set_fov(game, game->fov / 1.03, false);
	if (key_pressed(game, K_FOV_RESET))
		player_set_fov(game, 0, true);
	player_control_borders(game);
	game->p.vect.x = cos(game->p.angle);
	game->p.vect.y = sin(game->p.angle);
}

int	hook_key_press(int key_code, t_game *game)
{
	key_code = (short)key_code;
	if (key_code >= (int) sizeof(game->key) || key_code < -256)
		return (1);
	if (key_code == XK_1)
		game->test = !game->test;
	if (key_code == (short)K_EXIT)
		terminate(game, EXIT_SUCCESS, NULL);
	game->key[key_code + KEYCODE_OFFSET] = true;
	return (0);
}

int	hook_key_release(int key_code, t_game *game)
{
	key_code = (short)key_code;
	if (key_code >= (int) sizeof(game->key) || key_code < -256)
		return (1);
	game->key[key_code + KEYCODE_OFFSET] = false;
	return (0);
}

int	hook_exit(t_game *game)
{
	return (terminate(game, 0, NULL));
}

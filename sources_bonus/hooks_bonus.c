/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:33:10 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/01 16:21:08 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int	hook_key_press(int key_code, t_game *game)
{
	player_control_toggler(game, key_code);
	if (key_code == K_EXIT)
		terminate(game, EXIT_SUCCESS, NULL);
	key_code = (short)key_code;
	if (key_code >= (int) sizeof(game->key) || key_code < -256)
		return (1);
	game->key.k[key_code + KEYCODE_OFFSET] = true;
	return (0);
}

int	hook_key_release(int key_code, struct s_key *key)
{
	key_code = (short)key_code;
	if (key_code >= (int) sizeof(key->k) || key_code < -256)
		return (1);
	key->k[key_code + KEYCODE_OFFSET] = false;
	return (0);
}

int	hook_mouse_press(int btn, int x, int y, struct s_key *key)
{
	(void)x;
	(void)y;
	if ((unsigned)btn >= sizeof(key->m))
		return (1);
	key->m[btn] = true;
	return (0);
}

int	hook_mouse_release(int btn, int x, int y, struct s_key *key)
{
	(void)x;
	(void)y;
	if ((unsigned)btn >= sizeof(key->m))
		return (1);
	key->m[btn] = false;
	return (0);
}

int	hook_exit(t_game *game)
{
	return (terminate(game, 0, NULL));
}

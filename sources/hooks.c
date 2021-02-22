/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 22:16:42 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/07 18:51:15 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	hook_key_press(int key_code, struct s_key *key)
{
	if (key_code >= (int)sizeof(key->k))
		return (1);
	if (key_code == KEY_ESCAPE)
		terminate(EXIT_SUCCESS, NULL);
	key->k[key_code] = true;
	return (0);
}

int	hook_key_release(int key_code, struct s_key *key)
{
	if (key_code >= (int)sizeof(key->k))
		return (1);
	key->k[key_code] = false;
	return (0);
}

int	hook_mouse_press(int btn, int x, int y, struct s_key *key)
{
	(void)x;
	(void)y;
	if (btn >= (int)sizeof(key->m))
		return (1);
	key->m[btn] = true;
	return (0);
}

int	hook_mouse_release(int btn, int x, int y, struct s_key *key)
{
	(void)x;
	(void)y;
	if (btn >= (int)sizeof(key->m))
		return (1);
	key->m[btn] = false;
	return (0);
}

int	hook_mouse_move(int x, int y, struct s_key *key)
{
	key->mdir.x = x - key->mpos.x;
	key->mdir.y = y - key->mpos.y;
	key->mpos.x = x;
	key->mpos.y = y;
	return (0);
}

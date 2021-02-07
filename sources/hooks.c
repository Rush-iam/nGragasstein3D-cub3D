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

int	hook_key_press(int key, struct s_key *input)
{
	if (key >= (int)sizeof(input->k))
		return (1);
	if (key == KEY_ESCAPE)
		terminate();
	input->k[key] = true;
	return (0);
}

int	hook_key_release(int key, struct s_key *input)
{
	if (key >= (int)sizeof(input->k))
		return (1);
	input->k[key] = false;
	return (0);
}

int	hook_mouse_press(int button, int x, int y, struct s_key *input)
{
	(void)x;
	(void)y;
	if (button >= (int)sizeof(input->m))
		return (1);
	input->m[button] = true;
	return (0);
}

int	hook_mouse_release(int button, int x, int y, struct s_key *input)
{
	(void)x;
	(void)y;
	if (button >= (int)sizeof(input->m))
		return (1);
	input->m[button] = false;
	return (0);
}

int	hook_mouse_move(int x, int y, struct s_key *input)
{
	input->mdir.x = x - input->mpos.x;
	input->mdir.y = y - input->mpos.y;
	input->mpos.x = x;
	input->mpos.y = y;
	return (0);
}

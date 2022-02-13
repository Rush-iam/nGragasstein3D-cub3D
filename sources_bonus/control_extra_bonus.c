/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_extra_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 14:28:13 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 14:28:13 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

static inline bool	key_pressed(t_game *game, int key)
{
	return (game->key.k[(short)key + KEYCODE_OFFSET]);
}

void	player_control_weapon(t_game *game)
{
	if (game->p.weapon.frame == 0)
	{
		if (key_pressed(game, K_KNIFE) && game->p.weapon_cur != W_KNIFE)
			weapon_set(game, W_KNIFE);
		else if (key_pressed(game, K_PISTOL))
			weapon_set(game, W_PISTOL);
		else if (key_pressed(game, K_RIFLE))
			weapon_set(game, W_RIFLE);
	}
	if ((game->key.m[M_SHOOT] || key_pressed(game, K_SHOOT)) && \
		game->p.weapon.lock == false && \
		(game->p.ammo || game->p.weapon_cur == W_KNIFE))
	{
		game->p.weapon.lock = true;
		game->p.weapon.tick = ANIM_TICKS - 1;
	}
	if ((game->key.m[M_SHOOT] == false && key_pressed(game, K_SHOOT) == false) && \
		game->p.weapon.lock == true && game->p.weapon.frame == 0)
		game->p.weapon.lock = false;
}

void	player_control_fov(t_game *g)
{
	if (key_pressed(g, K_FOV_TELE))
		g->fov_reset = g->fov / 1.1f;
	else if (key_pressed(g, K_FOV_WIDE))
		g->fov_reset = g->fov * 1.1f;
	else if (key_pressed(g, K_FOV_RESET))
		g->fov_reset = ((g->img.aspect >= 1.77f) - (g->img.aspect < 1.77f)) * \
			sqrtf(fabsf(M_PI_4_F * (g->img.aspect - 1.77f) / 2.0f)) + M_PI_2_F;
	if (g->key.m[M_ZOOM_IN] && g->p.weapon_cur != W_KNIFE)
		g->fov_target = FOV_ZOOM_IN;
	else if (g->key.m[M_ZOOM_OUT] && g->p.weapon_cur != W_KNIFE)
		g->fov_target = FOV_ZOOM_OUT;
	else
		g->fov_target = g->fov_reset;
	if (fabsf(g->fov_target - g->fov) > FLOAT_FIX)
		player_set_fov(g, \
					g->fov + (g->fov_target - g->fov) / FOV_ZOOM_SPEED, false);
}

//	printf("FOV = %.1f\n", 114 * atanf(game->col_step * game->center.x));
void	player_set_fov(t_game *game, float fov, bool reset)
{
	int	n;

	if (reset)
		fov = game->fov_reset;
	game->col_step = tanf(fov / (game->img.size.x - 1));
	game->col_scale = 1.0f / game->col_step;
	n = 0;
	while (n < (int)game->img.size.x)
	{
		game->angles[n] = atanf(game->col_step * (n - game->center.x));
		n++;
	}
	if (reset == true || (M_PI_4_F / 4.0f < fov && fov < PI2_F))
		game->fov = fov;
}

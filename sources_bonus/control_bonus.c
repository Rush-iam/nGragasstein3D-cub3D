/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:31:39 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 14:22:18 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	player_control(t_game *game)
{
	player_control_rotate(game);
	player_control_move(game);
	player_control_jump_n_crouch(game);
	player_control_weapon(game);
	player_control_fov(game);
	game->p.pos.x = fminf(fmaxf(game->p.pos.x, 1.0f), game->map.size.x - 1);
	game->p.pos.y = fminf(fmaxf(game->p.pos.y, 1.0f), game->map.size.y - 1);
	player_control_borders(game);
	player_control_borders_diag(game);
	player_control_borders_enemies(game);
}

void	player_control_rotate(t_game *game)
{
	if (game->key.mouse == true)
	{
		mlx_mouse_get_pos(game->win, &game->key.mpos.x, &game->key.mpos.y);
		game->key.mdir.x = game->key.mdir.x / 2 + game->key.mpos.x - \
						   (int)game->resolution.x / 2;
		game->key.mdir.y = game->key.mdir.y / 2 + game->key.mpos.y - \
						   (int)game->resolution.y / 2;
		mlx_mouse_move(game->win, game->center.x, (int)game->resolution.y / 2);
		game->p.angle += game->key.mdir.x * game->col_step * MOUSE_SPEED;
		game->horizon -= game->key.mdir.y * MOUSE_SPEED;
		game->horizon = ft_min(3 * (int)game->img.size.y, \
						ft_max(-3 * (int)game->img.size.y, game->horizon));
	}
	if (game->key.k[K_TURN_LEFT])
		game->p.angle -= PL_SPEED / 2.0f;
	if (game->key.k[K_TURN_RIGHT])
		game->p.angle += PL_SPEED / 2.0f;
	if (game->p.angle >= M_PI_F)
		game->p.angle -= PI2_F;
	else if (game->p.angle < -M_PI_F)
		game->p.angle += PI2_F;
	__sincosf(game->p.angle, &game->p.vect.y, &game->p.vect.x);
}

void	player_control_move(t_game *g)
{
	const int	active = (g->key.k[K_MOVE_FORWARD] || g->key.k[K2_MOVE_FORWARD]
		|| g->key.k[K_MOVE_BACK] || g->key.k[K2_MOVE_BACK] ||
	g->key.k[K_MOVE_LEFT] || g->key.k[K_MOVE_RIGHT]) * (1 + g->key.k[K_RUN]);
	const float	target = fmaxf(0.0f, active - g->key.k[K_CROUCH] / 2.0f);
	const float	pl_speed = g->p.velocity * PL_SPEED;

	if (fabsf(target - g->p.velocity) > 0.01f)
		g->p.velocity += (target - g->p.velocity) / 5.0f;
	else
		g->p.velocity = target;
	if (g->key.k[K_MOVE_FORWARD] || g->key.k[K2_MOVE_FORWARD])
		g->p.pos = (t_fpoint){g->p.pos.x + pl_speed * g->p.vect.x,
			g->p.pos.y + pl_speed * g->p.vect.y};
	else if (g->key.k[K_MOVE_BACK] || g->key.k[K2_MOVE_BACK])
		g->p.pos = (t_fpoint){g->p.pos.x - pl_speed / 1.5f * g->p.vect.x,
			g->p.pos.y - pl_speed / 1.5f * g->p.vect.y};
	if (g->key.k[K_MOVE_LEFT])
		g->p.pos = (t_fpoint){g->p.pos.x + pl_speed * g->p.vect.y,
			g->p.pos.y - pl_speed * g->p.vect.x};
	if (g->key.k[K_MOVE_RIGHT])
		g->p.pos = (t_fpoint){g->p.pos.x - pl_speed * g->p.vect.y,
			g->p.pos.y + pl_speed * g->p.vect.x};
}

void	player_control_jump_n_crouch(t_game *g)
{
	g->z_level_target = PL_CROUCH_Z * g->key.k[K_CROUCH];
	if (g->z_level_vy > PL_GRAVITY)
	{
		g->z_level = fmaxf(0.0f, g->z_level + g->z_level_vy);
		if (g->z_level > 0.0f)
			g->z_level_vy += PL_GRAVITY / TICKS_PER_SEC;
		else
			g->z_level_vy = PL_GRAVITY;
	}
	else if (g->z_level < 0.0f || g->key.k[K_CROUCH] == true)
		g->z_level -= \
				(g->z_level - g->z_level_target - FLOAT_FIX) / PL_CROUCH_SPEED;
	else
		g->z_level = 0.0f;
}

void	player_control_toggler(t_game *g, int key_code)
{
	if (key_code == K_JUMP && fabsf(g->z_level) < 0.05f)
		g->z_level_vy = PL_JUMP;
	if (key_code == K_MOUSE_TOGGLE)
	{
		if (g->key.mouse == true)
			mlx_mouse_show();
		if (g->key.mouse == false)
		{
			mlx_mouse_move(g->win, g->resolution.x / 2, g->resolution.y / 2);
			mlx_mouse_hide();
		}
		g->key.mouse = !g->key.mouse;
	}
	if (key_code == K_MAP_TOGGLE)
		g->map.enabled = !g->map.enabled;
	if (key_code == K_USE)
		door_open_try(g, (t_point){g->p.pos.x + g->p.vect.x, \
									g->p.pos.y + g->p.vect.y}, &g->p.pos, true);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:31:39 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/05 15:14:22 by ngragas          ###   ########.fr       */
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

#ifdef __APPLE__

static inline void	mouse_get_pos(void *mlx_ptr, void *win_ptr, int *x, int *y)
{
	(void)mlx_ptr;
	mlx_mouse_get_pos(win_ptr, x, y);
}

static inline void	mouse_move(void *mlx_ptr, void *win_ptr, int x, int y)
{
	(void)mlx_ptr;
	mlx_mouse_move(win_ptr, x, y);
}

static inline void	mouse_show(void *mlx_ptr, void *win_ptr)
{
	(void)mlx_ptr;
	(void)win_ptr;
	mlx_mouse_show();
}

static inline void	mouse_hide(void *mlx_ptr, void *win_ptr)
{
	(void)mlx_ptr;
	(void)win_ptr;
	mlx_mouse_hide();
}

#else

static inline void	mouse_get_pos(void *mlx_ptr, void *win_ptr, int *x, int *y)
{
	mlx_mouse_get_pos(mlx_ptr, win_ptr, x, y);
}

static inline void	mouse_move(void *mlx_ptr, void *win_ptr, int x, int y)
{
	mlx_mouse_move(mlx_ptr, win_ptr, x, y);
}

static inline void	mouse_show(void *mlx_ptr, void *win_ptr)
{
	mlx_mouse_show(mlx_ptr, win_ptr);
}

static inline void	mouse_hide(void *mlx_ptr, void *win_ptr)
{
	mlx_mouse_hide(mlx_ptr, win_ptr);
}

#endif

static inline bool	key_pressed(t_game *game, int key)
{
	return (game->key.k[(short)key + KEYCODE_OFFSET]);
}

void	player_control_rotate(t_game *game)
{
	if (game->key.mouse == true)
	{
		mouse_get_pos(game->mlx, game->win, &game->key.mpos.x, &game->key.mpos.y);
		game->key.mdir.x = game->key.mdir.x / 2 + game->key.mpos.x - \
						   (int)game->resolution.x / 2;
		game->key.mdir.y = game->key.mdir.y / 2 + game->key.mpos.y - \
						   (int)game->resolution.y / 2;
		mouse_move(game->mlx, game->win, game->center.x, (int)game->resolution.y / 2);
		game->p.angle += game->key.mdir.x * game->col_step * MOUSE_SPEED;
		game->horizon -= game->key.mdir.y * MOUSE_SPEED;
		game->horizon = ft_min(3 * (int)game->img.size.y, \
						ft_max(-3 * (int)game->img.size.y, game->horizon));
	}
	if (key_pressed(game, K_TURN_LEFT))
		game->p.angle -= PL_SPEED / 2.0f;
	if (key_pressed(game, K_TURN_RIGHT))
		game->p.angle += PL_SPEED / 2.0f;
	if (game->p.angle >= M_PI_F)
		game->p.angle -= PI2_F;
	else if (game->p.angle < -M_PI_F)
		game->p.angle += PI2_F;
	game->p.vect.x = cosf(game->p.angle);
	game->p.vect.y = sinf(game->p.angle);
}

void	player_control_move(t_game *g)
{
	const int	active = (key_pressed(g, K_MOVE_FORWARD) || key_pressed(g, K2_MOVE_FORWARD)
		|| key_pressed(g, K_MOVE_BACK) || key_pressed(g, K2_MOVE_BACK) ||
	key_pressed(g, K_MOVE_LEFT) || key_pressed(g, K_MOVE_RIGHT)) * (1 + key_pressed(g, K_RUN));
	const float	target = fmaxf(0.0f, active - key_pressed(g, K_CROUCH) / 2.0f);
	const float	pl_speed = g->p.velocity * PL_SPEED;

	if (fabsf(target - g->p.velocity) > 0.01f)
		g->p.velocity += (target - g->p.velocity) / 5.0f;
	else
		g->p.velocity = target;
	if (key_pressed(g, K_MOVE_FORWARD) || key_pressed(g, K2_MOVE_FORWARD))
		g->p.pos = (t_fpoint){g->p.pos.x + pl_speed * g->p.vect.x,
			g->p.pos.y + pl_speed * g->p.vect.y};
	else if (key_pressed(g, K_MOVE_BACK) || key_pressed(g, K2_MOVE_BACK))
		g->p.pos = (t_fpoint){g->p.pos.x - pl_speed / 1.5f * g->p.vect.x,
			g->p.pos.y - pl_speed / 1.5f * g->p.vect.y};
	if (key_pressed(g, K_MOVE_LEFT))
		g->p.pos = (t_fpoint){g->p.pos.x + pl_speed * g->p.vect.y,
			g->p.pos.y - pl_speed * g->p.vect.x};
	if (key_pressed(g, K_MOVE_RIGHT))
		g->p.pos = (t_fpoint){g->p.pos.x - pl_speed * g->p.vect.y,
			g->p.pos.y + pl_speed * g->p.vect.x};
}

void	player_control_jump_n_crouch(t_game *g)
{
	g->z_level_target = 0.5f + PL_CROUCH_Z * key_pressed(g, K_CROUCH);
	if (g->z_level_vy > PL_GRAVITY)
	{
		g->z_level = fmaxf(0.5f, g->z_level + g->z_level_vy);
		if (g->z_level > 0.5f)
			g->z_level_vy += PL_GRAVITY / TICKS_PER_SEC;
		else
			g->z_level_vy = PL_GRAVITY;
	}
	else if (g->z_level < 0.5f || key_pressed(g, K_CROUCH) == true)
		g->z_level -= \
				(g->z_level - g->z_level_target - FLOAT_FIX) / PL_CROUCH_SPEED;
	else
		g->z_level = 0.5f;
}

void	player_control_toggler(t_game *g, int key_code)
{
	if (key_code == K_JUMP && fabsf(g->z_level) < 0.55f)
		g->z_level_vy = PL_JUMP;
	if (key_code == K_MOUSE_TOGGLE)
	{
		if (g->key.mouse == true)
			mouse_show(g->mlx, g->win);
		if (g->key.mouse == false)
		{
			mouse_move(g->mlx, g->win, g->resolution.x / 2, g->resolution.y / 2);
			mouse_hide(g->mlx, g->win);
		}
		g->key.mouse = !g->key.mouse;
	}
	if (key_code == K_MAP_TOGGLE)
		g->map.enabled = !g->map.enabled;
	if (key_code == K_USE)
		door_open_try(g, (t_point){g->p.pos.x + g->p.vect.x, \
                                    g->p.pos.y + g->p.vect.y}, true);
}

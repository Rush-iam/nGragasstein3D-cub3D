/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:31:39 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/30 15:32:40 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	player_control(t_game *game)
{
	player_control_rotate(game);
	player_control_move(game);
	player_control_jump_n_crouch(game);
	player_control_weapon(game);
	player_control_extra(game);
	game->p.pos.x = fmax(game->p.pos.x, 1);
	game->p.pos.y = fmax(game->p.pos.y, 1);
	game->p.pos.x = fmin(game->p.pos.x, game->map.size.x - 1);
	game->p.pos.y = fmin(game->p.pos.y, game->map.size.y - 1);
//	player_control_borders(game);
}

void	player_control_rotate(t_game *game)
{
	if (game->key.mouse == true)
	{
		mlx_mouse_get_pos(game->win, &game->key.mpos.x, &game->key.mpos.y);
		game->key.mdir.x = game->key.mdir.x / 2 + game->key.mpos.x -
						   game->resolution.x / 2;
		game->key.mdir.y = game->key.mdir.y / 2 + game->key.mpos.y -
						   game->resolution.y / 2;
		mlx_mouse_move(game->win, game->center.x, game->resolution.y / 2);
		game->p.angle += game->key.mdir.x / MOUSE_SPEED_X;
		game->horizon -= game->key.mdir.y / 1.5f;
		game->horizon = ft_min(3 * game->img.size.y,
						ft_max(-3 * game->img.size.y, game->horizon));
	}
	if (game->key.k[K_TURN_LEFT])
		game->p.angle -= PL_SPEED / 3.0f;
	if (game->key.k[K_TURN_RIGHT])
		game->p.angle += PL_SPEED / 3.0f;
	if (game->p.angle >= M_PI_F)
		game->p.angle -= PI2_F;
	else if (game->p.angle < -M_PI_F)
		game->p.angle += PI2_F;
	__sincosf(game->p.angle, &game->p.vect.y, &game->p.vect.x);
}

void	player_control_move(t_game *game)
{
	const float	pl_speed = PL_SPEED - (game->z_level < -0.1f) * PL_SPEED / 2.0f;

	if (game->key.k[K_MOVE_FORWARD])
		game->p.pos = (t_fpoint){
		game->p.pos.x + (game->key.k[K_RUN] + 1) * pl_speed * game->p.vect.x,
		game->p.pos.y + (game->key.k[K_RUN] + 1) * pl_speed * game->p.vect.y};
	if (game->key.k[K_MOVE_BACK])
		game->p.pos = (t_fpoint){
	game->p.pos.x - (game->key.k[K_RUN] + 1) * pl_speed / 1.5f * game->p.vect.x,
	game->p.pos.y - (game->key.k[K_RUN] + 1) * pl_speed / 1.5f * game->p.vect.y};
	if (game->key.k[K_MOVE_LEFT])
		game->p.pos = (t_fpoint){
		game->p.pos.x + (game->key.k[K_RUN] + 1) * pl_speed * game->p.vect.y,
		game->p.pos.y - (game->key.k[K_RUN] + 1) * pl_speed * game->p.vect.x};
	if (game->key.k[K_MOVE_RIGHT])
		game->p.pos = (t_fpoint){
		game->p.pos.x - (game->key.k[K_RUN] + 1) * pl_speed * game->p.vect.y,
		game->p.pos.y + (game->key.k[K_RUN] + 1) * pl_speed * game->p.vect.x};
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
	{
		g->z_level -=
				(g->z_level - g->z_level_target - FLOAT_FIX) / PL_CROUCH_SPEED;
	}
	else
		g->z_level = 0.0f;
}

void	player_control_weapon(t_game *game)
{
	if (game->p.weapon.frame == 0)
	{
		if (game->key.k[K_KNIFE] && game->p.weapon_cur != W_KNIFE)
			player_set_weapon(game, W_KNIFE);
		else if (game->key.k[K_PISTOL])
			player_set_weapon(game, W_PISTOL);
		else if (game->key.k[K_RIFLE])
			player_set_weapon(game, W_RIFLE);
	}
	if (game->key.m[M_SHOOT] && game->p.weapon.lock == false &&
		(game->p.ammo || game->p.weapon_cur == W_KNIFE))
	{
		game->p.weapon.lock = true;
		game->p.weapon.tick = ANIM_TICKS - 1;
	}
	if (game->key.m[M_SHOOT] == false && game->p.weapon.lock == true
		&& game->p.weapon.frame == 0)
		game->p.weapon.lock = false;
}

void	player_set_fov(t_game *game, float fov, bool reset)
{
	int n;

	if (reset)
		fov = ((game->img.aspect >= 1.77f) - (game->img.aspect < 1.77f)) *
		  sqrtf(fabsf(M_PI_4_F * (game->img.aspect - 1.77f) / 2.0f)) + M_PI_2_F;
	game->col_step = tanf(fov / (game->img.size.x - 1));
	game->col_scale = 1 / game->col_step;
	n = 0;
	while (n < (int)game->img.size.x)
	{
		game->angles[n] =
				atanf(game->col_step * (n - game->center.x));
		n++;
	}
	if (reset == false)
		printf("FOV = %.1f\n", 114 * atanf(game->col_step * game->center.x));
	if (reset == true || (M_PI_4_F / 4.0f < fov && fov < PI2_F))
		game->fov = fov;
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
		door_open(g,
		(t_point){g->p.pos.x + g->p.vect.x, g->p.pos.y + g->p.vect.y}, true);
}

void	player_control_extra(t_game *game)
{
	if (game->key.k[K_FOV_WIDE])
		player_set_fov(game, game->fov * FOV_ZOOMSPEED, false);
	if (game->key.k[K_FOV_TELE])
		player_set_fov(game, game->fov / FOV_ZOOMSPEED, false);
	if (game->key.k[K_FOV_RESET])
		player_set_fov(game, 0, true);
}

void	player_control_borders_enemies(t_game *game)
{
	t_list		*cur_list;
	t_object	*obj;
	t_fpoint	diff;

	cur_list = game->objects;
	while (cur_list)
	{
		obj = (t_object *)cur_list->content;
		diff = (t_fpoint){obj->pos.x - game->p.pos.x, obj->pos.y - game->p.pos.y};
		obj->distance_real = hypotf(diff.x, diff.y);
		if (obj->distance_real < PL_RADIUS * 2 &&
			obj->type == T_ENEMY && obj->e->state != ST_DEATH)
		{
			obj->atan_diff = atan2f(diff.y, diff.x);
			obj->distance = game->p.vect.x * diff.x + game->p.vect.y * diff.y;
			game->p.pos.x = obj->pos.x - PL_RADIUS * 2 * cosf(obj->atan_diff);
			game->p.pos.y = obj->pos.y - PL_RADIUS * 2 * sinf(obj->atan_diff);
		}
		cur_list = cur_list->next;
	}
}

void	player_control_borders(t_game *g)
{
	const t_upoint	plus = {g->p.pos.x + PL_RADIUS, g->p.pos.y + PL_RADIUS};
	const t_upoint	minus = {g->p.pos.x - PL_RADIUS, g->p.pos.y - PL_RADIUS};
	char			check;

	check = g->map.grid[(int)g->p.pos.y][minus.x];
	if (check != '.' && (chr_is_wall(check) || check == CHAR_SOLID_MAP ||
			(ft_memchr(CHAR_DOORS, check, sizeof(CHAR_DOORS)) && door_find(g,
					(t_point){minus.x, (int)g->p.pos.y})->part_opened < 0.5f)))
		g->p.pos.x = minus.x + 1 + PL_RADIUS + FLOAT_FIX;
	check = g->map.grid[(int)g->p.pos.y][plus.x];
	if (check != '.' && (chr_is_wall(check) || check == CHAR_SOLID_MAP ||
			(ft_memchr(CHAR_DOORS, check, sizeof(CHAR_DOORS)) && door_find(g,
					(t_point){plus.x, (int)g->p.pos.y})->part_opened < 0.5f)))
		g->p.pos.x = plus.x - PL_RADIUS - FLOAT_FIX;
	check = g->map.grid[minus.y][(int)g->p.pos.x];
	if (check != '.' && (chr_is_wall(check) || check == CHAR_SOLID_MAP ||
			(ft_memchr(CHAR_DOORS, check, sizeof(CHAR_DOORS)) && door_find(g,
					(t_point){(int)g->p.pos.x, minus.y})->part_opened < 0.5f)))
		g->p.pos.y = minus.y + 1 + PL_RADIUS + FLOAT_FIX;
	check = g->map.grid[plus.y][(int)g->p.pos.x];
	if (check != '.' && (chr_is_wall(check) || check == CHAR_SOLID_MAP ||
			(ft_memchr(CHAR_DOORS, check, sizeof(CHAR_DOORS)) && door_find(g,
					(t_point){(int)g->p.pos.x, plus.y})->part_opened < 0.5f)))
		g->p.pos.y = plus.y - PL_RADIUS - FLOAT_FIX;
	player_control_borders_diag(g);
	player_control_borders_enemies(g);
}

void	player_control_borders_diag(t_game *g)
{
	const t_upoint	plus = {g->p.pos.x + PL_RADIUS, g->p.pos.y + PL_RADIUS};
	const t_upoint	minus = {g->p.pos.x - PL_RADIUS, g->p.pos.y - PL_RADIUS};

	if (chr_is_wall(g->map.grid[minus.y][minus.x]))
		(g->p.pos.x - (int)g->p.pos.x > g->p.pos.y - (int)g->p.pos.y) ?
		(g->p.pos.x = minus.x + 1 + PL_RADIUS) :
		(g->p.pos.y = minus.y + 1 + PL_RADIUS);
	if (chr_is_wall(g->map.grid[plus.y][plus.x]))
		(g->p.pos.x - (int)g->p.pos.x < g->p.pos.y - (int)g->p.pos.y) ?
		(g->p.pos.x = plus.x - PL_RADIUS) :
		(g->p.pos.y = plus.y - PL_RADIUS);
	if (chr_is_wall(g->map.grid[minus.y][plus.x]))
		(1.f - (g->p.pos.x - (int)g->p.pos.x) > g->p.pos.y - (int)g->p.pos.y) ?
		(g->p.pos.x = plus.x - PL_RADIUS) :
		(g->p.pos.y = minus.y + 1 + PL_RADIUS);
	if (chr_is_wall(g->map.grid[plus.y][minus.x]))
		(g->p.pos.x - (int)g->p.pos.x > 1.f - (g->p.pos.y - (int)g->p.pos.y)) ?
		(g->p.pos.x = minus.x + 1 + PL_RADIUS) :
		(g->p.pos.y = plus.y - PL_RADIUS);
}

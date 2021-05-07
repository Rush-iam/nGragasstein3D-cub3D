/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/05 22:37:16 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/07 12:56:53 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	initialize_game(t_game *game, bool screenshot_only)
{
	initialize_sounds(game);
	bfs_grid_initialize(game);
	initialize_window(game, screenshot_only);
	initialize_hud_images(game);
	initialize_face_images(game);
	initialize_canvas_images(game);
	game->hud.face_pos = (t_point){game->hud.bar.size.x * HUD_FACE_X, \
						game->img.size.y + game->hud.bar.size.y * HUD_FACE_Y};
	initialize_weapons_and_faded_walls(game);
	initialize_map_hud(game);
	if (!ft_malloc_ptr((void **)&game->column, \
			game->img.size.x * sizeof(*game->column)))
		terminate(game, ERR_MEM, "Memory allocation failed (ray columns)");
	if (!ft_malloc_ptr((void **)&game->angles, \
			game->img.size.x * sizeof(*game->angles)))
		terminate(game, ERR_MEM, "Memory allocation failed (ray angles)");
	initialize_values(game);
	music_play(game, &game->audio.music[MUSIC_BG_ID]);
}

void	initialize_window(t_game *game, bool screenshot_only)
{
	t_point	max_res;

	if (screenshot_only)
		max_res = (t_point){MAX_SCREENSHOT_X, MAX_SCREENSHOT_Y};
	else
	{
		mlx_get_screen_size(&max_res.x, &max_res.y);
		max_res.y -= 44;
	}
	game->resolution.x = ft_umax(game->resolution.x & ~1U, MIN_RES);
	game->resolution.x = ft_umin(game->resolution.x, max_res.x);
	game->resolution.y = ft_umin(game->resolution.y & ~1U, max_res.y);
	if (screenshot_only == false)
	{
		game->win = mlx_new_window(
				game->mlx, game->resolution.x, game->resolution.y, TITLE);
		if (game->win == NULL)
			terminate(game, ERR_MEM, strerror(errno));
		mlx_mouse_move(game->win, game->resolution.x / 2, \
									game->resolution.y / 2);
	}
}

void	initialize_values(t_game *game)
{
	if (!game->fade_distance)
		game->fade_distance = 8;
	__sincosf(game->p.angle, &game->p.vect.y, &game->p.vect.x);
	game->horizon = game->center.y;
	game->z_level = 0.5f;
	game->fov_reset = \
		((game->img.aspect >= 1.77f) - (game->img.aspect < 1.77f)) * \
		sqrtf(fabsf(M_PI_4_F * (game->img.aspect - 1.77f) / 2.0f)) + M_PI_2_F;
	player_set_fov(game, 0, true);
	game->key.mouse = true;
	game->p.health = START_HEALTH;
	game->p.ammo = START_AMMO;
	game->p.weapons_mask = START_WEAPONS;
	weapon_set(game, W_KNIFE);
	weapon_set(game, W_PISTOL);
}

void	initialize_hooks(t_game *game)
{
	mlx_do_key_autorepeatoff(game->mlx);
	mlx_mouse_hide();
	mlx_hook(game->win, EVENT_KEYPRESS, 0, hook_key_press, game);
	mlx_hook(game->win, EVENT_KEYRELEASE, 0, hook_key_release, &game->key);
	mlx_hook(game->win, EVENT_BUTTONPRESS, 0, hook_mouse_press, &game->key);
	mlx_hook(game->win, EVENT_BUTTONRELEASE, 0, hook_mouse_release, &game->key);
	mlx_hook(game->win, EVENT_DESTROYNOTIFY, 0, hook_exit, game);
	mlx_loop_hook(game->mlx, game_loop, game);
}
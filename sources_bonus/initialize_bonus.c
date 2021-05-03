/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/05 22:37:16 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 15:32:53 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	initialize_game(t_game *g, bool screenshot_only)
{
	initialize_sounds(g);
	bfs_grid_initialize(g);
	initialize_window(g, screenshot_only);
	initialize_hud_images(g);
	initialize_face_images(g);
	initialize_canvas_images(g);
	g->hud.face_pos = (t_point){g->hud.bar.size.x * HUD_FACE_X, \
						g->img.size.y + g->hud.bar.size.y * HUD_FACE_Y};
	initialize_weapons_and_faded_walls(g);
	initialize_map_hud(g);
	if (!ft_malloc_ptr((void **)&g->column, g->img.size.x * sizeof(*g->column)))
		terminate(g, ERR_MEM, "Memory allocation failed (ray columns)");
	if (!ft_malloc_ptr((void **)&g->angles, g->img.size.x * sizeof(*g->angles)))
		terminate(g, ERR_MEM, "Memory allocation failed (ray angles)");
	__sincosf(g->p.angle, &g->p.vect.y, &g->p.vect.x);
	g->fov_reset = ((g->img.aspect >= 1.77f) - (g->img.aspect < 1.77f)) * \
			sqrtf(fabsf(M_PI_4_F * (g->img.aspect - 1.77f) / 2.0f)) + M_PI_2_F;
	player_set_fov(g, 0, true);
	g->key.mouse = true;
	g->p.health = START_HEALTH;
	g->p.ammo = START_AMMO;
	g->p.weapons_mask = START_WEAPONS;
	weapon_set(g, W_KNIFE);
	weapon_set(g, W_PISTOL);
	music_play(g, &g->audio.music[MUSIC_BG_ID]);
}

void	initialize_window(t_game *g, bool screenshot_only)
{
	t_point	max_res;

	if (screenshot_only)
		max_res = (t_point){MAX_SCREENSHOT_X, MAX_SCREENSHOT_Y};
	else
	{
		mlx_get_screen_size(&max_res.x, &max_res.y);
		max_res.y -= 44;
	}
	g->resolution.x = ft_umax(g->resolution.x & ~1U, MIN_RES);
	g->resolution.x = ft_umin(g->resolution.x, max_res.x);
	g->resolution.y = ft_umin(g->resolution.y & ~1U, max_res.y);
	if (screenshot_only == false)
	{
		g->win = mlx_new_window(
				g->mlx, g->resolution.x, g->resolution.y, TITLE);
		if (g->win == NULL)
			terminate(g, ERR_MEM, strerror(errno));
		mlx_mouse_move(g->win, g->resolution.x / 2, g->resolution.y / 2);
	}
}

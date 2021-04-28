/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/05 22:37:16 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/28 15:55:56 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	initialize_game(t_game *game, bool screenshot_only)
{
	initialize_sounds(game);
	initialize_bfs_grid(game);
	initialize_window(game, screenshot_only);
	initialize_hud_images(game);
	initialize_face_images(game);
	initialize_canvas_images(game);
	game->hud.face_pos = (t_point){game->hud.bar.size.x * HUD_FACE_X,
						game->img.size.y + game->hud.bar.size.y * HUD_FACE_Y};
	initialize_weapons_and_faded_walls(game);
	initialize_map_hud(game);
	if (!(game->column = ft_calloc(game->img.size.x, sizeof(*game->column))))
		terminate(game, ERR_MEM, "Memory allocation failed (ray columns)");
	if (!(game->angles = malloc(game->img.size.x * sizeof(*game->angles))))
		terminate(game, ERR_MEM, "Memory allocation failed (ray angles)");
	__sincosf(game->p.angle, &game->p.vect.y, &game->p.vect.x);
	player_set_fov(game, 0, true);
	game->key.mouse = true;
	game->p.health = START_HEALTH;
	game->p.ammo = START_AMMO;
	game->p.weapons_mask = START_WEAPONS;
	player_set_weapon(game, W_KNIFE);
	player_set_weapon(game, W_PISTOL);
	if (game->audio.music[0].file.channels[0])
		sound_play(game, &game->audio.music[0], T_FPT_NULL)->looped = true;
}

void	initialize_window(t_game *g, bool screenshot_only)
{
	t_point	max_res;

	if (screenshot_only)
		max_res = MAX_SCREENSHOT;
	else
	{
		mlx_get_screen_size(&max_res.x, &max_res.y);
		max_res.y -= 44;
	}
	g->resolution.x = ft_umin(ft_umax(g->resolution.x & ~1U, MIN_RES), max_res.x);
	g->resolution.y = ft_umin(g->resolution.y & ~1U, max_res.y);
	if (screenshot_only == false)
	{
		g->win = mlx_new_window(
				g->mlx, g->resolution.x, g->resolution.y, WIN_TITLE);
		if (g->win == NULL)
			terminate(g, ERR_MEM, strerror(errno));
		mlx_mouse_move(g->win, g->resolution.x / 2, g->resolution.y / 2);
	}
}

void	initialize_hud_images(t_game *g)
{
	const char	*weapons[] = {HUD_KNIFE_FILE, HUD_PISTOL_FILE, HUD_RIFLE_FILE};
	unsigned	i;

	load_image_file(g, HUD_BAR_FILE, &g->hud.bar,
					"Error loading HUD status bar image file");
	g->hud.scale = (float)g->resolution.x / g->hud.bar.size.x;
	if (img_resize(g->mlx, &g->hud.bar, (t_upoint)
			{g->resolution.x, g->resolution.x / g->hud.bar.aspect}) == NULL)
		terminate(g, ERR_MEM, strerror(errno));
	img_from_file_scaled(g, HUD_DIGITS_FILE, &g->hud.digits, g->hud.scale);
	g->hud.digit_width = (float)g->hud.digits.size.x / (sizeof(HUD_DIGITS) - 1);
	i = 0;
	while (i < sizeof(weapons) / sizeof(*weapons))
	{
		img_from_file_scaled(g, (char *)weapons[i], &g->hud.weapon[i],
																g->hud.scale);
		i++;
	}
	g->hud.needs_redraw = true;
}

void	initialize_face_images(t_game *g)
{
	int		lvl;
	int		dir;
	char	*path;

	lvl = 0;
	while (lvl < HUD_FACE_LEVELS)
	{
		dir = 0;
		while (dir < HUD_FACE_DIRS)
		{
			path = ft_strjoin(HUD_FACE_FILES,
				(char []){lvl + '0', dir + '0', '.', 'p', 'n', 'g', '\0'});
			if (path == NULL)
				terminate(g, ERR_MEM, "Failed to create path to HUD face file");
			img_from_file_scaled(g, path, &g->hud.face[lvl][dir], g->hud.scale);
			free(path);
			dir++;
		}
		lvl++;
	}
	path = ft_strjoin(HUD_FACE_FILES, "dead.png");
	if (path == NULL)
		terminate(g, ERR_MEM, "Failed to create path to HUD face file");
	img_from_file_scaled(g, path, &g->hud.face_dead, g->hud.scale);
	free(path);
}

void	initialize_canvas_images(t_game *g)
{
	if (img_create(g->mlx, &g->img, (t_upoint){g->resolution.x, ft_max(
			(int)g->resolution.y - (int)g->hud.bar.size.y, MIN_RES)}) == NULL)
		terminate(g, ERR_MEM, strerror(errno));
	g->img_center = (t_upoint){g->img.size.x / 2, g->img.size.y / 2};
	g->img_bytecount = g->img.size.x * g->img.size.y * 4;
	if (img_create(g->mlx, &g->img_bg, g->img.size) == NULL)
		terminate(g, ERR_MEM, strerror(errno));
	img_ceilfloor_rgb_faded(&g->img_bg, g->color_ceil, g->color_floor,
														g->fade_distance);
	if (img_create(g->mlx, &g->img_effect, g->img.size) == NULL)
		terminate(g, ERR_MEM, strerror(errno));
}

void	initialize_weapons_and_faded_walls(t_game *g)
{
	unsigned	i;
	unsigned	n;
	const int	textures_count = sizeof(g->texture) / sizeof(*g->texture) / 2;
	t_upoint	weapon_size;

	i = 0;
	while (i < sizeof(g->p.weapon_img) / sizeof(*g->p.weapon_img))
	{
		n = 0;
		while (n < sizeof(g->p.weapon_img[i]) / sizeof(*g->p.weapon_img[i]))
		{
			weapon_size.y = g->img.size.y * 2 / 3;
			weapon_size.x = weapon_size.y * g->p.weapon_img[i][n].aspect;
			if (img_resize(g->mlx, &g->p.weapon_img[i][n++], weapon_size) == NULL)
				terminate(g, ERR_MEM, strerror(errno));
		}
		i++;
	}
	i = 0;
	while (i < textures_count)
	{
		g->texture[i + textures_count] = img_faded_copy(g->mlx, &g->texture[i]);
		if (g->texture[i++ + textures_count].ptr == NULL)
			terminate(g, ERR_MEM, strerror(errno));
	}
}

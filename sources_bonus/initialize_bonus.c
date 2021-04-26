/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/05 22:37:16 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/26 17:22:27 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"


void	initialize_game(t_game *game, bool screenshot_only)
{
	int			n;
	const int	n_tex = sizeof(game->texture) / sizeof(*game->texture) / 2;

	initialize_game_images(game, screenshot_only);
	initialize_weapons_scale(game);
	initialize_bfs_grid(game);
	n = 0;
	while (n < n_tex)
	{
		game->texture[n + n_tex] = img_faded_copy(game->mlx, &game->texture[n]);
		if (game->texture[n + n_tex].ptr == NULL)
			terminate(game, ERR_MLX, strerror(errno));
		n++;
	}
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

void	initialize_game_images(t_game *g, bool screenshot_only)
{
	t_point	max_res;

	if (screenshot_only)
		max_res = MAX_SCR;
	else
		mlx_get_screen_size(&max_res.x, &max_res.y);
	g->img.size.x = ft_umax(g->img.size.x & ~1U, MIN_RES_X);
	g->img.size.x = ft_min((int)g->img.size.x & ~1U, max_res.x);
	g->img.size.y = ft_min((int)g->img.size.y & ~1U, max_res.y);
	g->win_center = (t_upoint){g->img.size.x / 2, g->img.size.y / 2};
	if (screenshot_only == false)
	{
		if (!(g->win = mlx_new_window(
				g->mlx, g->img.size.x, g->img.size.y, WIN_TITLE)))
			terminate(g, ERR_MLX, strerror(errno));
		mlx_mouse_move(g->win, g->win_center.x, g->win_center.y);
	}
	if (!img_create(g->mlx, &g->img, g->img.size.x, g->img.size.y))
		terminate(g, ERR_MLX, strerror(errno));
	if (!img_create(g->mlx, &g->img_effect, g->img.size.x, g->img.size.y))
		terminate(g, ERR_MLX, strerror(errno));
	if (!img_create(g->mlx, &g->img_bg, g->img.size.x, g->img.size.y))
		terminate(g, ERR_MLX, strerror(errno));
	img_ceilfloor_rgb_faded(&g->img_bg, g->color_ceil, g->color_floor,
							g->fade_distance);
}

void	initialize_weapons_scale(t_game *game)
{
	unsigned	i;
	unsigned	n;
	t_upoint	weapon_size;

	i = 0;
	while (i < sizeof(game->p.weapon_img) / sizeof(*game->p.weapon_img))
	{
		n = 0;
		while (n < 4)
		{
			weapon_size.y = game->img.size.y * 2 / 3;
			weapon_size.x = weapon_size.y * game->p.weapon_img[i][n].aspect;
			game->p.weapon_img[i][n] = img_resize(game->mlx,
												  &game->p.weapon_img[i][n], weapon_size);
			if (game->p.weapon_img[i][n].ptr == NULL)
				terminate(game, ERR_MLX, strerror(errno));
			n++;
		}
		i++;
	}
}

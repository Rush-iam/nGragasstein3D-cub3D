/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/05 22:37:16 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/16 00:14:47 by ngragas          ###   ########.fr       */
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

//	game->lastlevel = true; //!!!!!!!!!
	if (game->lastlevel)
		sound_play(game, &game->audio.sound[SND_EMITSOUND],
									(t_fpoint){53.3f, 6.2f})->looped = true;
}

void	initialize_game_images(t_game *game, bool screenshot_only)
{
	const t_upoint	max_res = (screenshot_only == true) ? MAX_SCR : MAX_WIN;
	int				n;

	game->img.size.x = ft_umax(game->img.size.x & ~1U, MIN_RES_X);
	game->img.size.x = ft_umin(game->img.size.x & ~1U, max_res.x);
	game->img.size.y = ft_umin(game->img.size.y & ~1U, max_res.y);
	game->img.aspect = (float)game->img.size.x / game->img.size.y;
	game->win_center = (t_upoint){game->img.size.x / 2, game->img.size.y / 2};
	if (screenshot_only == false)
	{
		if (!(game->win = mlx_new_window(
				game->mlx, game->img.size.x, game->img.size.y, WIN_TITLE)))
			terminate(game, ERR_MLX, strerror(errno));
		mlx_mouse_move(game->win, game->win_center.x, game->win_center.y);
	}
	if (!(game->img.ptr = mlx_new_image(
			game->mlx, game->img.size.x, game->img.size.y)))
		terminate(game, ERR_MLX, strerror(errno));
	game->img.data = (void *)mlx_get_data_addr(game->img.ptr, &n, &n, &n);
	if (!(game->effect_img.ptr = mlx_new_image(
			game->mlx, game->img.size.x, game->img.size.y)))
		terminate(game, ERR_MLX, strerror(errno));
	game->effect_img.data = (unsigned *)mlx_get_data_addr(game->effect_img.ptr,
														  &n, &n, &n);
	game->effect_img.size = game->img.size;
	if (!(game->img_bg.ptr = mlx_new_image(
			game->mlx, game->img.size.x, game->img.size.y)))
		terminate(game, ERR_MLX, strerror(errno));
	game->img_bg.data = (unsigned *)mlx_get_data_addr(game->img_bg.ptr,
													  &n, &n, &n);
	game->img_bg.size = game->img.size;
	img_ceilfloor_rgb_faded(&game->img_bg, game->color_ceil, game->color_floor,
							game->fade_distance);
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

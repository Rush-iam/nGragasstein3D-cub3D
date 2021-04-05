/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:33:07 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/05 19:54:16 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CUTE_SOUND_IMPLEMENTATION
#include "cub3d_bonus.h"

int	main(int args, char *av[])
{
	t_game					game;
	bool					screenshot_only;
	static struct timespec	time;

	game = (t_game){0};
	game.fade_distance = 8;
	if (!(game.mlx = mlx_init()))
		terminate(&game, ERR_MLX, strerror(errno));
	game.audio.ctx = cs_make_context(0, 44100, 8192, 2, NULL);
	game.audio.ctx7 = cs_make_context(0, 7042, 1024, MAX_PLAYING_SOUNDS, NULL);
	game.audio.ctx22 = cs_make_context(0, 22050, 2048, MAX_PLAYING_SOUNDS, NULL);
	parse(args, av, &game, &screenshot_only);
	initialize_game(&game, screenshot_only);
	draw_map_init(&game);
	if (screenshot_only == true)
		write_screenshot_and_exit(&game);
	mlx_do_key_autorepeatoff(game.mlx);
	mlx_mouse_hide();
	mlx_hook(game.win, EVENT_KEYPRESS, 0, hook_key_press, &game);
	mlx_hook(game.win, EVENT_KEYRELEASE, 0, hook_key_release, &game.key);
	mlx_hook(game.win, EVENT_BUTTONPRESS, 0, hook_mouse_press, &game.key);
	mlx_hook(game.win, EVENT_BUTTONRELEASE, 0, hook_mouse_release, &game.key);
	mlx_hook(game.win, EVENT_DESTROYNOTIFY, 0, hook_exit, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	clock_gettime(CLOCK_MONOTONIC, &time);
	game.tick = 60 * time.tv_sec + 60 * time.tv_nsec / NANSECS_PER_SEC;
	mlx_loop(game.mlx);
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

void	initialize_game(t_game *game, bool screenshot_only)
{
	int			n;
	const int	n_tex = sizeof(game->texture) / sizeof(*game->texture) / 2;

	initialize_game_images(game, screenshot_only);
	initialize_weapons_scale(game);
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

int	dead_exit(t_game *game)
{
	game->effect.frame_cur++;
	if (game->effect.type != EF_FIZZLEFADE)
	{
		game->effect = (struct s_effect){384, 0, EF_FIZZLEFADE, COLOR_RED, 0};
		cs_stop_all_sounds(game->audio.ctx);
		cs_stop_all_sounds(game->audio.ctx7);
		cs_stop_all_sounds(game->audio.ctx22);
		sound_play(game, &game->audio.sound[SND_PLAYER_DEATH], T_FPT_NULL);
	}
	draw_effect(game, &game->effect);
	if (game->effect.frame_cur > game->effect.frames)
		terminate(game, 0, NULL);
	return (0);
}

int	game_loop(t_game *game)
{
	static clock_t			clock_cur;
	static struct timespec	time;
	static unsigned			tick_prev;
	int						fps;

	clock_gettime(CLOCK_MONOTONIC, &time);
	tick_prev = game->tick;
	game->tick = TICKS_PER_SEC * time.tv_sec +
				TICKS_PER_SEC * time.tv_nsec / NANSECS_PER_SEC;
	game->tick_diff = game->tick - tick_prev;
	sounds(game);
	if (game->p.health < 0)
		return dead_exit(game);
	if (game->effect.frame_cur < game->effect.frames)
		game->effect.frame_cur += game->tick_diff;
	while (game->tick_diff > 0)
	{
		doors(game);
		player_control(game);
		objects(game);
		weapon(game, &game->p.weapon);
//		for (int i = 0; i < 100; ++i)
		ray_cast(game);
		game->tick_diff--;
	}
//	img_ceilfloor_rgb(&game->img, game->color_ceil, game->color_floor);
	mlx_put_image_to_window(game->mlx, game->win, game->img_bg.ptr, 0, 0);
	img_clear_rgb(&game->img, -1U);
	draw_walls(game);
	draw_objects(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.ptr, 0, 0);
	draw_effect(game, &game->effect);
	draw_weapon(game, &game->p.weapon);
	draw_map(game);

//	demo_fillrate(game, 1);
//	demo_cursor(game, 0xFF88FF);
//	demo_radar(game, 360);
	fps = CLOCKS_PER_SEC / (clock() - clock_cur);
	clock_cur = clock();
	mlx_string_put(game->mlx, game->win, 0, 10, COLOR_WHITE,
		(char []){'0' + fps/100, '0' + fps / 10 % 10, '0' + fps % 10, '\0'});
	return (0);
}

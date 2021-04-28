/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:33:07 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/28 20:24:44 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	parse(args, av, &game, &screenshot_only);
	initialize_game(&game, screenshot_only);
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

void	draw_ceilfloor(t_game *g)
{
	const int	pixel_offset = (g->horizon - g->center.y) * (int)g->img.size.x;
	int 		i;

	if (pixel_offset < 0)
	{
		if (-pixel_offset < g->img_pixelcount)
			ft_memcpy(g->img.data, g->img_bg.data - pixel_offset,
						(g->img_pixelcount + pixel_offset) * 4);
		i = ft_max(0, g->img_pixelcount + pixel_offset);
		while (i < g->img_pixelcount)
			g->img.data[i++] = g->color_floor;
	}
	else
	{
		i = ft_min(g->img_pixelcount, pixel_offset);
		while (i >= 0)
			g->img.data[i--] = g->color_ceil;
		if (pixel_offset < g->img_pixelcount)
			ft_memcpy(g->img.data + pixel_offset, g->img_bg.data,
						(g->img_pixelcount - pixel_offset) * 4);
	}
}

int	game_loop(t_game *game)
{
	if (game->p.health == 0)
		return dead_exit(game);
	game_ticks(game);
	sounds(game);
	draw_ceilfloor(game);
	draw_walls(game);
	draw_objects(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.ptr, 0, 0);
	draw_effect(game, &game->effect);
	draw_weapon(game, &game->p.weapon);
	if (game->string.text)
		draw_string(game, &game->string);
	if (game->map.enabled)
		draw_map(game);
	if (game->hud.needs_redraw)
		draw_hud(game);
	draw_hud_face(game, false);
	draw_fps(game);
//	demo_fillrate(game, 1);
//	demo_cursor(game, 0xFF88FF);
//	demo_radar(game, 360);
	return (0);
}

void	game_ticks(t_game *game)
{
	static struct timespec	time;
	static unsigned			tick_prev;

	clock_gettime(CLOCK_MONOTONIC, &time);
	tick_prev = game->tick;
	game->tick = TICKS_PER_SEC * time.tv_sec +
				 TICKS_PER_SEC * time.tv_nsec / NANSECS_PER_SEC;
	game->tick_diff = game->tick - tick_prev;
	if (game->effect.frame_cur < game->effect.frames)
		game->effect.frame_cur += game->tick_diff;
	if (game->string.frame_cur < game->string.frames)
		game->string.frame_cur += game->tick_diff;
	while (game->tick_diff > 0)
	{
		doors(game);
		player_control(game);
		weapon(game, &game->p.weapon);
		objects(game);
//		for (int i = 0; i < 100; ++i)
		ray_cast(game);
		game->tick_diff--;
	}
}

int	dead_exit(t_game *game)
{
	sounds(game);
	game->effect.frame_cur++;
	if (game->effect.type != EF_FIZZLEFADE)
	{
		game->effect = (struct s_effect){0, 384, EF_FIZZLEFADE, COLOR_RED, 0};
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

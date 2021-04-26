/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:33:07 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/26 23:19:54 by ngragas          ###   ########.fr       */
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

void	draw_string(t_game *g, struct s_string *s)
{
	float	fade;

	if (s->text)
	{
		if (s->frame_cur < s->frames)
		{
			fade = 1.0f;
			if (s->fade == true)
			{
				if (s->frame_cur < TICKS_PER_SEC)
					fade = (float)s->frame_cur / TICKS_PER_SEC;
				else if (s->frames - s->frame_cur < TICKS_PER_SEC)
					fade = (float)(s->frames - s->frame_cur) / TICKS_PER_SEC;
			}
			mlx_string_put(g->mlx, g->win, s->pos.x, s->pos.y,
						   pixel_fade(s->color, fade), s->text);
		}
		else
		{
			free(s->text);
			s->text = NULL;
		}
	}
}

void	string_add(t_game *g, char *string, int timer, unsigned color)
{
	free(g->string.text);
	g->string.text = string;
	g->string.pos.x = g->img.size.x / 2 - ft_strlen(string) * 7;
	g->string.pos.y = g->img.size.y / 2 + 14;
	g->string.frame_cur = 0;
	g->string.frames = timer * TICKS_PER_SEC;
	g->string.fade = (timer >= 3);
	g->string.color = color;
}

void	draw_fps(t_game *game)
{
	static clock_t	clock_cur;
	int				fps;

	fps = CLOCKS_PER_SEC / (clock() - clock_cur);
	clock_cur = clock();
	mlx_string_put(game->mlx, game->win, 0, 20, COLOR_WHITE,
		(char []){'0' + fps/100, '0' + fps / 10 % 10, '0' + fps % 10, '\0'});
}

void	game_tick(t_game *game)
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

void draw_hud(t_game *game)
{
	mlx_put_image_to_window(game->mlx, game->win, game->hud.bar_clean.ptr, 0,
								game->img.size.y - game->hud.bar.size.y);
	mlx_put_image_to_window(game->mlx, game->win, game->hud.digits.ptr, 0, 0);
	mlx_string_put(game->mlx, game->win,
				game->img.size.x - 260, game->img.size.y - 8, COLOR_CYAN_F,
		(char []){'H', 'e', 'a', 'l', 't', 'h', ':',
		'0' + game->p.health / 100, '0' + game->p.health / 10 % 10,
		'0' + game->p.health % 10, '\0'});
	mlx_string_put(game->mlx, game->win,
				game->img.size.x - 110, game->img.size.y - 8, COLOR_CYAN_F,
		(char []){'A', 'm', 'm', 'o', ':',
		'0' + game->p.ammo / 10 % 10, '0' + game->p.ammo % 10, '\0'});
}

int	game_loop(t_game *game)
{
	if (game->p.health < 0)
		return dead_exit(game);
	game_tick(game);
	sounds(game);
//	img_ceilfloor_rgb(&game->img, game->color_ceil, game->color_floor);
	mlx_put_image_to_window(game->mlx, game->win, game->img_bg.ptr, 0, 0);
	img_clear_rgb(&game->img, -1U);
	draw_walls(game);
	draw_objects(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.ptr, 0, 0);
	draw_effect(game, &game->effect);
	draw_weapon(game, &game->p.weapon);
	draw_map(game);
	draw_string(game, &game->string);
	draw_hud(game);
	draw_fps(game);
//	demo_fillrate(game, 1);
//	demo_cursor(game, 0xFF88FF);
//	demo_radar(game, 360);
	return (0);
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

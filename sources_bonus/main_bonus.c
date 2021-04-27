/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:33:07 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/27 23:40:37 by ngragas          ###   ########.fr       */
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

void	draw_string(t_game *g, struct s_string *s)
{
	float	fade;

	if (s->pos.x == 0)
	{
		g->string.pos.x = g->img.size.x / 2 - ft_strlen(s->text) * 7;
		g->string.pos.y = g->img.size.y / 2 + 14;
	}
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

void	string_add(t_game *g, char *string, int timer, unsigned color)
{
	free(g->string.text);
	g->string.text = string;
	g->string.pos.x = g->img.size.x / 2 - ft_strlen(string) * 7;
	g->string.pos.y = g->img.size.y / 2 + 14;
	if (g->img.size.x == 0)
		g->string.pos.x = 0;
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

char	*hud_num_to_str(char num_str[11], unsigned num, int length)
{
	if (length == 0)
		return (num_str);
	num_str[length] = '\0';
	num_str[length - 1] = '0';
	if (num == 0)
		length--;
	while (num && length--)
	{
		num_str[length] = '0' + num % 10;
		num /= 10;
	}
	while (length-- > 0)
		num_str[length] = ':';
	return (num_str);
}

void	hud_digits_bake(struct s_hud *hud, char num_str[], t_upoint pos)
{
	int			i;
	int			src_x_offset;
	t_upoint	src_px;

	i = 0;
	while (num_str[i])
	{
		src_x_offset = (num_str[i] - '0') * hud->digit_width + 1;
		src_px.y = 0;
		while (src_px.y < hud->digits.size.y)
		{
			src_px.x = 0;
			while (src_px.x < (unsigned)hud->digit_width)
			{
				hud->bar.data[(pos.y + src_px.y) * hud->bar.size.x + pos.x +
										src_px.x + i * (int)hud->digit_width] =
					hud->digits.data[src_px.y * hud->digits.size.x +
												src_x_offset + src_px.x];
				src_px.x++;
			}
			src_px.y++;
		}
		i++;
	}
}

void	draw_hud(t_game *g)
{
	char		num_str[11];
	const float	y_offset = g->hud.bar.size.y * HUD_VALUES_OFFSET_Y;

	hud_digits_bake(&g->hud, hud_num_to_str(num_str, g->p.score, 6),
						(t_upoint){g->hud.bar.size.x * HUD_SCORE_X, y_offset});
	hud_digits_bake(&g->hud, hud_num_to_str(num_str, 1, 1),
						(t_upoint){g->hud.bar.size.x * HUD_FLOOR_X, y_offset});
	hud_digits_bake(&g->hud, hud_num_to_str(num_str, 1, 1),
						(t_upoint){g->hud.bar.size.x * HUD_LIVES_X, y_offset});
	hud_digits_bake(&g->hud, hud_num_to_str(num_str, g->p.health, 3),
						(t_upoint){g->hud.bar.size.x * HUD_HEALTH_X, y_offset});
	hud_digits_bake(&g->hud, hud_num_to_str(num_str, g->p.ammo, 2),
						(t_upoint){g->hud.bar.size.x * HUD_AMMO_X, y_offset});
	mlx_put_image_to_window(g->mlx, g->win, g->hud.bar.ptr, 0, g->img.size.y);
	mlx_put_image_to_window(g->mlx, g->win, g->hud.weapon[g->p.weapon_cur].ptr,
		g->hud.bar.size.x * HUD_WEAPON_X,
		g->img.size.y + g->hud.bar.size.y * HUD_WEAPON_Y);
	g->hud.needs_redraw = false;
}

//void	draw_face(t_game *game)
//{
//	const unsigned	health_level = 100 - game->p.health / ;
//
//	health_level = ft_umin(sizeof(game->hud.face));
//	mlx_put_image_to_window(game->mlx, game->win, game->hud.face[])
//}

int	game_loop(t_game *game)
{
	if (game->p.health == 0)
		return dead_exit(game);
	game_tick(game);
	sounds(game);
	ft_memcpy(game->img.data, game->img_bg.data, game->img_bytecount);
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
//	if (game->tick % HUD_FACE_TICK_TIMER == 0)
//		draw_face(game);
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

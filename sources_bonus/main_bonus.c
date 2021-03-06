/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:33:07 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/07 12:58:15 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int	main(int args, char *av[])
{
	t_game			game;
	bool			screenshot_only;
	struct timespec	time;

	game = (t_game){};
	game.fade_distance = 8;
	if (args == 1)
		terminate(&game, ERR_ARGS, "Please specify scene filename");
	else if (args >= 4)
		terminate(&game, ERR_ARGS, "Too many arguments");
	game.color_floor = -1U;
	game.color_ceil = -1U;
	if (ft_assign_ptr(&game.mlx, mlx_init()) == NULL)
		terminate(&game, ERR_MLX, strerror(errno));
	screenshot_only = parse(args, ++av, &game);
	initialize_game(&game, screenshot_only);
	if (screenshot_only)
		write_screenshot_and_exit(&game);
	initialize_hooks(&game);
	clock_gettime(CLOCK_MONOTONIC, &time);
	game.tick = 60 * time.tv_sec + 60 * time.tv_nsec / NANSECS_PER_SEC - 1;
	srandom(time.tv_nsec);
	mlx_loop(game.mlx);
}

int	game_loop(t_game *game)
{
	if (game->p.health == 0)
		return (dead_exit(game));
	game_ticks(game);
	sounds(game);
	if (game->color_ceil != -1U)
		draw_ceil_plain(game);
	if (game->color_floor != -1U)
		draw_floor_plain(game);
	ray_cast(game, -1);
	draw_walls(game);
	draw_objects(game);
	put_image_to_window(game->mlx, game->win, game->img.ptr, (t_point){0, 0});
	draw_effect(game, &game->effect);
	draw_weapon(game, &game->p.weapon);
	if (game->map.enabled)
		draw_map(game);
	draw_hud(game);
	draw_hud_face(game, false);
	if (*game->string.text)
		draw_message(game, &game->string);
	draw_fps(game);
	return (0);
}
//	demo_fillrate(game, 1);
//	demo_cursor(game, 0xFF88FF);
//	demo_radar(game, 360);

void	game_ticks(t_game *game)
{
	static struct timespec	time;
	static uint32_t			tick_prev;

	clock_gettime(CLOCK_MONOTONIC, &time);
	tick_prev = game->tick;
	game->tick = TICKS_PER_SEC * time.tv_sec + \
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
		game->tick_diff--;
	}
}

int	dead_exit(t_game *game)
{
	sounds(game);
	game->effect.frame_cur++;
	if (game->effect.type != EF_FIZZLEFADE)
	{
		game->effect = (struct s_effect){0, 192, EF_FIZZLEFADE, COLOR_RED, 0};
		if (game->audio.disabled == false)
		{
			cs_stop_all_sounds(game->audio.ctx);
			cs_stop_all_sounds(game->audio.ctx7);
			cs_stop_all_sounds(game->audio.ctx22);
		}
		sound_play(game, &game->audio.sound[SND_PLAYER_DEATH], \
					(t_fpoint){0, 0});
	}
	draw_effect(game, &game->effect);
	if (game->effect.frame_cur > game->effect.frames)
		terminate(game, 0, NULL);
	return (0);
}

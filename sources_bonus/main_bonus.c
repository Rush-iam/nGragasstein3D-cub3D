/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:33:07 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/11 23:48:54 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int	main(int args, char *av[])
{
	t_game					game;
	bool					screenshot_only;
	static struct timespec	time;

	errno = 0;
	game = (t_game){0};
	if (!(game.mlx = mlx_init()))
		terminate(&game, ERR_MLX, strerror(errno));
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

void	initialize_game(t_game *game, bool screenshot)
{
	const t_upoint	max_res = (screenshot == true) ? MAX_SCR : MAX_WIN;
	int				n;

	if (game->img.size.x < MIN_RES_X)
		game->img.size.x = MIN_RES_X;
	if (game->img.size.x > max_res.x)
		game->img.size.x = max_res.x;
	if (game->img.size.y > max_res.y)
		game->img.size.y = max_res.y;
	game->img.aspect = (double)game->img.size.x / game->img.size.y;
	if (screenshot == false)
		if (!(game->win = mlx_new_window(
				game->mlx, game->img.size.x, game->img.size.y, WIN_TITLE)))
			terminate(game, ERR_MLX, strerror(errno));
	if (!(game->img.ptr = mlx_new_image(
			game->mlx, game->img.size.x, game->img.size.y)))
		terminate(game, ERR_MLX, strerror(errno));
	game->img.data = (unsigned *)mlx_get_data_addr(game->img.ptr, &n, &n, &n);
	if (!(game->effect_img.ptr = mlx_new_image(
			game->mlx, game->img.size.x, game->img.size.y)))
		terminate(game, ERR_MLX, strerror(errno));
	game->effect_img.data = (unsigned *)mlx_get_data_addr(game->effect_img.ptr,
																	&n, &n, &n);
	game->effect_img.size = game->img.size;
	if (!(game->column = malloc(sizeof(struct s_column *) * game->img.size.x)))
		terminate(game, ERR_MEM, "Memory allocation failed (ray columns)");
	n = 0;
	while (n < (int)game->img.size.x)
		if ((game->column[n++] = ft_calloc(1, sizeof(struct s_column))) == NULL)
			terminate(game, ERR_MEM, "Memory allocation failed (column)");
	initialize_game_2(game);
}

void	initialize_game_2(t_game *game)
{
	t_list		*cur_list;
	t_object	*obj;

	__sincos(game->p.angle, &game->p.vector.y, &game->p.vector.x);
	game->win_center = (t_upoint){game->img.size.x / 2, game->img.size.y / 2};
	mlx_mouse_move(game->win, game->win_center.x, game->win_center.y);
	set_fov(game, 0, true);
	game->p.health = 100;
	game->p.ammo = 8;
	player_set_weapon(game, W_PISTOL);
	game->p.weapon_cur = W_PISTOL;
	game->p.weapons = WEAPON_KNIFE | WEAPON_PISTOL;
	cur_list = game->objects;
	while (cur_list)
	{
		obj = (t_object *)cur_list->content;
		obj->distance = game->p.vector.x * (obj->pos.x - game->p.pos.x) +
						game->p.vector.y * (obj->pos.y - game->p.pos.y);
		cur_list = cur_list->next;
	}
}

void	weapon_shoot(t_game *game)
{
	(void)game;
}

void	weapon(t_game *game, struct s_weapon *weapon)
{
	if (weapon->tick > 0)
	{
		if (++weapon->tick >= weapon->ticks)
		{
			weapon->tick = 0;
			weapon->frame = 0;
		}
		weapon->frame = weapon->frames * weapon->tick / weapon->ticks;
		if (weapon->frame == 2 && game->p.weapon_shot == false)
		{
			weapon_shoot(game);
			game->p.weapon_shot = true;
		}
		else if (weapon->frame == 3)
			game->p.weapon_shot = false;
		if (game->p.weapon_cur == W_RIFLE && weapon->frame == 3)
			weapon->lock = false;
	}
}

void	draw_weapon(t_game *game, struct s_weapon *weapon)
{
	mlx_put_image_to_window(game->mlx, game->win, game->p.weapon_img
				[game->p.weapon_cur][weapon->animation[weapon->frame]].ptr,
							game->p.weapon_pos.x, game->p.weapon_pos.y);
}

int	game_loop(t_game *game)
{
	static clock_t			clock_cur;
	static struct timespec	time;
	static unsigned			tick_prev;
	int						fps;

	clock_gettime(CLOCK_MONOTONIC, &time);
	tick_prev = game->tick;
	game->tick = 60 * time.tv_sec + 60 * time.tv_nsec / NANSECS_PER_SEC;
	game->tick_diff = game->tick - tick_prev;
	if (game->effect.frame_cur < game->effect.frames)
		game->effect.frame_cur += game->tick_diff;
	while (game->tick_diff > 0)
	{
		player_control(game);
		objects(game);
		weapon(game, &game->p.weapon);
		game->tick_diff--;
	}
//	for (int i = 0; i < 500; ++i)
		ray_cast(game);
	img_ceilfloor_rgb(&game->img, game->color_ceil, game->color_floor);
	draw_walls(game);
	draw_objects(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.ptr, 0, 0);
	draw_effect(game, &game->effect);
	draw_weapon(game, &game->p.weapon);
	draw_map(game);

//	demo_cursor(game, 0xFF88FF);
//	demo_fillrate(game, 1);
//	fizzlefade(&game->img, 0xFF0000);
//	demo_radar(game, 360);
	fps = CLOCKS_PER_SEC / (clock() - clock_cur);
	clock_cur = clock();
	mlx_string_put(game->mlx, game->win, 0, 10, COLOR_WHITE,
		(char []){'0' + fps/100, '0' + fps / 10 % 10, '0' + fps % 10, '\0'});
	return (0);
}

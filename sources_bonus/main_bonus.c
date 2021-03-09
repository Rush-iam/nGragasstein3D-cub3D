/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:33:07 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/09 22:32:04 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int	main(int args, char *av[])
{
	t_game	game;
	bool	screenshot_only;

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
	game->p.weapon_cur = WEAPON_PISTOL;
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

void	set_fov(t_game *game, double fov, bool reset)
{
	if (reset)
		fov = ((game->img.aspect >= 1.77) - (game->img.aspect < 1.77)) *
					sqrt(fabs(M_PI_4 * (game->img.aspect - 1.77) / 2)) + M_PI_2;
	game->col_step = tan(fov / (game->img.size.x - 1));
	game->col_center = (float)game->img.size.x / 2;
	game->col_scale = 1 / game->col_step;
	printf("Real FOV: %.0f\n", 114 * atan(game->col_step * game->col_center));
	if (reset == true || (M_PI_4 / 4 < fov && fov < PI2))
		game->fov = fov;
}

void	effect_flash(t_game *game, unsigned color, float power)
{
	img_clear_rgb(&game->effect_img, color | ((int)(255 - 255. * power) << 24));
	mlx_put_image_to_window(game->mlx, game->win, game->effect_img.ptr, 0, 0);
}

void	draw_effect(t_game *game, struct s_effect *ef)
{
	float	power;

	if (ef->frame_cur < ef->frames)
	{
		ef->frame_cur += game->tick_diff;
		if (ef->frame_cur >= ef->frames)
			ef->frame_cur = ef->frames - 1;
		power = (float)ef->frame_cur / (ef->frames / 2.);
		if (power > 1)
			power = 2 - power;
		if (ef->type == EF_FLASH)
			effect_flash(game, game->effect.color, power * ef->max_power);
	}
}

int	game_loop(t_game *game)
{
	static clock_t			clock_cur;
	static struct timespec	time;
	static unsigned			tick_prev;
	char					*fps;

	clock_gettime(CLOCK_MONOTONIC, &time);
	tick_prev = game->tick;
	game->tick = 60 * time.tv_sec + 60 * time.tv_nsec / NANSECS_PER_SEC;
	game->tick_diff = game->tick - tick_prev;
	printf("tick: %u\n", game->tick);
	player_control(game);
//	for (int i = 0; i < 500; ++i)
		ray_cast(game);
	img_ceilfloor_rgb(&game->img, game->color_ceil, game->color_floor);
	draw_walls(game);
	objects(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.ptr, 0, 0);
	draw_effect(game, &game->effect);
	draw_map(game);

//	demo_cursor(game, 0xFF88FF);
//	demo_fillrate(game, 1);
//	fizzlefade(&game->img, 0xFF0000);
//	demo_radar(game, 360);
//	printf("FPS: %lu\n", CLOCKS_PER_SEC / (clock() - tick));
	fps = ft_itoa(CLOCKS_PER_SEC / (clock() - clock_cur));
	clock_cur = clock();
	mlx_string_put(game->mlx, game->win, 0, 10, 0xFFFFFF, fps);
	free(fps);

	return (0);
}

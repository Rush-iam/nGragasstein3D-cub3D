/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:58:52 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/03 23:01:00 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "x_events.h"

int	main(int args, char *av[])
{
	t_game	game;
	bool	screenshot_only;

	errno = 0;
	game = (t_game){0};
	if (!(game.mlx = mlx_init()))
		terminate(&game, ERR_MLX, strerror(errno));
	screenshot_only = parse(args, av, &game);
	validate_scene(&game);
	initialize_game(&game, screenshot_only);
	draw_map_init(&game); //
	if (screenshot_only == true)
		write_screenshot_and_exit(&game);
	mlx_do_key_autorepeatoff(game.mlx);
//	mlx_mouse_hide();
	mlx_hook(game.win, EVENT_KEYPRESS, 0, hook_key_press, &game);
	mlx_hook(game.win, EVENT_KEYRELEASE, 0, hook_key_release, &game.key);
	mlx_hook(game.win, EVENT_BUTTONPRESS, 0, hook_mouse_press, &game.key);
	mlx_hook(game.win, EVENT_BUTTONRELEASE, 0, hook_mouse_release, &game.key);
	mlx_hook(game.win, EVENT_DESTROYNOTIFY, 0, hook_exit, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_loop(game.mlx);
}

void	validate_scene(t_game *game)
{
	if (game->p.pos.x == 0)
		terminate(game, ERR_PARSE, "Player pos character 'NSWE' not found");
	if (game->color_floor == -1U)
		terminate(game, ERR_PARSE, "Floor color not found. Format: F R,G,B");
	if (game->color_ceil == -1U)
		terminate(game, ERR_PARSE, "Ceil color not found. Format: C R,G,B");
	if (game->img.size.x == 0 || game->img.size.y == 0)
		terminate(game, ERR_PARSE,
				  "Resolution doesn't set. Format: 'R WIDTH HEIGHT' (max 32767x32767)");
	if (game->texture[WALL_N].ptr == NULL)
		terminate(game, ERR_PARSE,
				  "North wall texture doesn't set. Format: 'NO ./path.xpm'");
	if (game->texture[WALL_S].ptr == NULL)
		terminate(game, ERR_PARSE,
				  "South wall texture doesn't set. Format: 'SO ./path.xpm'");
	if (game->texture[WALL_W].ptr == NULL)
		terminate(game, ERR_PARSE,
				  "West wall texture doesn't set. Format: 'WE ./path.xpm'");
	if (game->texture[WALL_E].ptr == NULL)
		terminate(game, ERR_PARSE,
				  "East wall texture doesn't set. Format: 'EA ./path.xpm'");
	if (game->texture[SPRITE].ptr == NULL)
		terminate(game, ERR_PARSE,
				  "Sprite texture doesn't set. Format: 'S ./path.xpm'");
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
	if (screenshot == false)
		if (!(game->win = mlx_new_window(
				game->mlx, game->img.size.x, game->img.size.y, WIN_TITLE)))
			terminate(game, ERR_MLX, strerror(errno));
	if (!(game->img.ptr = mlx_new_image(
			game->mlx, game->img.size.x, game->img.size.y)))
		terminate(game, ERR_MLX, strerror(errno));
	game->img.data = (unsigned *)mlx_get_data_addr(game->img.ptr, &n, &n, &n);
	if (!(game->column = malloc(sizeof(struct s_column *) * game->img.size.x)))
		terminate(game, ERR_MEM, "Memory allocation failed (ray columns)");
	n = 0;
	while (n < (int)game->img.size.x)
		if ((game->column[n++] = ft_calloc(1, sizeof(struct s_column))) == NULL)
			terminate(game, ERR_MEM, "Memory allocation failed (column)");
	initialize_game_objects(game);
}

void	initialize_game_objects(t_game *game)
{
	t_list		*cur_list;
	t_object	*obj;

	__sincos(game->p.angle, &game->p.cossin.y, &game->p.cossin.x);
	cur_list = game->objects;
	while (cur_list)
	{
		obj = (t_object *)cur_list->content;
		obj->distance = game->p.cossin.x * (obj->pos.x - game->p.pos.x) +
						game->p.cossin.y * (obj->pos.y - game->p.pos.y);
		cur_list = cur_list->next;
	}
}

int	game_loop(t_game *game)
{
	static clock_t	tick;
	char			*fps;

	player_control(game);
//	for (int i = 0; i < 500; ++i)
		ray_cast(game);
	img_ceilfloor_fill_rgb(&game->img, game->color_ceil, game->color_floor);
	draw_walls(game);
	draw_objects(game);

//	demo_cursor(game, 0xFF88FF);
//	demo_fillrate(game, 1);
	mlx_put_image_to_window(game->mlx, game->win, game->img.ptr, 0, 0);
	draw_map(game);

//	fizzlefade(&game->img, 0xFF0000);
//	demo_radar(game, 360);
//	ft_putnbr_fd(CLOCKS_PER_SEC / (clock() - tick), 1);
//	write(1, "\n", 1);
	fps = ft_itoa(CLOCKS_PER_SEC / (clock() - tick));
	tick = clock();
	mlx_string_put(game->mlx, game->win, 0, 10, 0xFFFFFF, fps);
	free(fps);

//	write(1, "Player x", 9); ft_putnbr_fd((int)game->p.pos.x, 1);
//	write(1, " y", 3); ft_putnbr_fd((int)game->p.pos.y, 1); write(1, "\n", 1);
//	write(1, "Column 0 height ", 16); ft_putnbr_fd(game->column[0].height, 1);
//	write(1, " cell x", 7); ft_putnbr_fd((int)game->column[0].cell.x, 1);
//	write(1, " y", 2); ft_putnbr_fd((int)game->column[0].cell.y, 1);
//	write(1, " dir=", 5); write(1, &(game->column[0].dir), 1); write(1, "\n", 1);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:58:52 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/28 18:44:20 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "x_events.h"

int		write_screenshot()
{
	return (EXIT_SUCCESS);
}

int	main(int args, char *av[])
{
	t_game	game;

	errno = 0;
	game = (t_game){0};
	if (!(game.mlx = mlx_init()))
		terminate(ERROR_MLX, strerror(errno));
	parse(args, av, &game);
	initialize(&game);
	mlx_do_key_autorepeatoff(game.mlx);
	mlx_hook(game.win, EVENT_KEYPRESS, 0, hook_key_press, &game.key);
	mlx_hook(game.win, EVENT_KEYRELEASE, 0, hook_key_release, &game.key);
	mlx_hook(game.win, EVENT_BUTTONPRESS, 0, hook_mouse_press, &game.key);
	mlx_hook(game.win, EVENT_BUTTONRELEASE, 0, hook_mouse_release, &game.key);
	mlx_hook(game.win, EVENT_MOTIONNOTIFY, 0, hook_mouse_move, &game.key);
	mlx_hook(game.win, EVENT_DESTROYNOTIFY, 0, terminate, NULL);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_loop(game.mlx);
}

void	initialize(t_game *game)
{
	t_upoint	max_res = {5400, 2200}; //
	t_upoint	min_res = {2, 1};
	int			n;

	if (game->img.size.x > max_res.x)
		game->img.size.x = max_res.x;
	if (game->img.size.y > max_res.y)
		game->img.size.y = max_res.y;
	if (game->img.size.x < min_res.x)
		game->img.size.x = min_res.x;
	if (!(game->win = mlx_new_window(
			game->mlx, game->img.size.x, game->img.size.y, WINDOW_TITLE)))
		terminate(ERROR_MLX, strerror(errno));
	if (!(game->img.ptr = mlx_new_image(
			game->mlx, game->img.size.x, game->img.size.y)))
		terminate(ERROR_MLX, strerror(errno));
	game->img.data = (unsigned *)mlx_get_data_addr(game->img.ptr, &n, &n, &n);
	if (!(game->column = malloc(sizeof(struct s_column *) * game->img.size.x)))
		terminate(ERROR_MEMORY, "Memory allocation failed (window columns)");
	n = 0;
	while (n < (int)game->img.size.x)
		if ((game->column[n++] = ft_calloc(1, sizeof(struct s_column))) == NULL)
			terminate(ERROR_MEMORY, "Memory allocation failed (single column)");
	initialize_objects_distance(game);
	draw_map_init(game);
}

void	initialize_objects_distance(t_game *game)
{
	t_list		*cur_list;
	t_object	*obj;
	t_fpoint	diff;

	__sincos(game->p.angle, &game->p.cossin.y, &game->p.cossin.x);
	cur_list = game->objects;
	while (cur_list)
	{
		obj = (t_object *)cur_list->content;
		diff = (t_fpoint){obj->pos.x - game->p.pos.x,
						  obj->pos.y - game->p.pos.y};
		obj->distance = game->p.cossin.x * diff.x + game->p.cossin.y * diff.y;
		cur_list = cur_list->next;
	}
}

int	game_loop(t_game *game)
{
	static clock_t	tick;
	char			*fps;

	player_control(game);
	img_ceilfloor_fill_rgb(&game->img, game->color_ceil, game->color_floor);
	ray_cast(game);
	draw_walls(game);
//	for (int i = 0; i < 250; ++i)
		draw_objects(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.ptr, 0, 0);
	draw_map(game);

//	fizzlefade(&game->img, 0xFF0000);
//	demo_fillrate(game, 1);
//	demo_radar(game, 360);
//	demo_cursor(game, 0xFF88FF);
//	ft_putnbr_fd(CLOCKS_PER_SEC / (clock() - start), 1);
//	write(1, "\n", 1);
	fps = ft_itoa(CLOCKS_PER_SEC / (clock() - tick));
	tick = clock();
	mlx_string_put(game->mlx, game->win, 0, 0, 0xFFFFFF, fps);
	free(fps);

//	write(1, "Player x", 9); ft_putnbr_fd((int)game->p.pos.x, 1);
//	write(1, " y", 3); ft_putnbr_fd((int)game->p.pos.y, 1); write(1, "\n", 1);
//	write(1, "Column 0 height ", 16); ft_putnbr_fd(game->column[0].height, 1);
//	write(1, " cell x", 7); ft_putnbr_fd((int)game->column[0].cell.x, 1);
//	write(1, " y", 2); ft_putnbr_fd((int)game->column[0].cell.y, 1);
//	write(1, " dir=", 5); write(1, &(game->column[0].dir), 1); write(1, "\n", 1);
	return (0);
}

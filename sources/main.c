/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:58:52 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 22:12:20 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	main(int args, char *av[])
{
	t_game	game;
	bool	screenshot_only;

	errno = 0;
	game = (t_game){0};
	if (args == 1)
		terminate(&game, ERR_ARGS, "Please specify scene filename");
	else if (args >= 4)
		terminate(&game, ERR_ARGS, "Too many arguments");
	game.mlx = mlx_init();
	if (game.mlx == NULL)
		terminate(&game, ERR_MLX, strerror(errno));
	parse(args, av, &game, &screenshot_only);
	initialize_game(&game, screenshot_only);
	if (screenshot_only == true)
		write_screenshot_and_exit(&game);
	mlx_do_key_autorepeatoff(game.mlx);
	mlx_hook(game.win, EVENT_KEYPRESS, 0, hook_key_press, &game);
	mlx_hook(game.win, EVENT_KEYRELEASE, 0, hook_key_release, &game);
	mlx_hook(game.win, EVENT_DESTROYNOTIFY, 0, hook_exit, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_loop(game.mlx);
}

void	initialize_game(t_game *g, bool screenshot)
{
	t_upoint	max_res;
	int			n;

	if (screenshot)
		max_res = (t_upoint){MAX_SCREENSHOT_X, MAX_SCREENSHOT_Y};
	else
		mlx_get_screen_size((int *)&max_res.x, (int *)&max_res.y);
	g->img.size.x = ft_umax(MIN_RES, g->img.size.x);
	g->img.size.x = ft_umin(max_res.x, g->img.size.x);
	g->img.size.y = ft_umin(max_res.y, g->img.size.y);
	g->img.aspect = (double)g->img.size.x / g->img.size.y;
	if (screenshot == false)
		g->win = mlx_new_window(g->mlx, g->img.size.x, g->img.size.y, TITLE);
	if (screenshot == false && g->win == NULL)
		terminate(g, ERR_MLX, strerror(errno));
	g->img.ptr = mlx_new_image(g->mlx, g->img.size.x, g->img.size.y);
	if (g->img.ptr == NULL)
		terminate(g, ERR_MLX, strerror(errno));
	g->img.data = (unsigned int *)mlx_get_data_addr(g->img.ptr, &n, &n, &n);
	g->column = malloc(g->img.size.x * sizeof(*g->column));
	if (g->column == NULL)
		terminate(g, ERR_MEM, "Memory allocation failed (ray columns)");
	initialize_game_2(g);
}

void	initialize_game_2(t_game *game)
{
	t_list		*cur_list;
	t_object	*obj;

	__sincos(game->p.angle, &game->p.vect.y, &game->p.vect.x);
	player_set_fov(game, 0, true);
	cur_list = game->objects;
	while (cur_list)
	{
		obj = (t_object *)cur_list->content;
		obj->distance = game->p.vect.x * (obj->pos.x - game->p.pos.x) + \
						game->p.vect.y * (obj->pos.y - game->p.pos.y);
		cur_list = cur_list->next;
	}
}

void	player_set_fov(t_game *game, float fov, bool reset)
{
	if (reset)
		fov = ((game->img.aspect >= 1.77) - (game->img.aspect < 1.77)) * \
			sqrt(fabs(M_PI_4 * (game->img.aspect - 1.77) / 2)) + M_PI_2;
	game->col_step = tan(fov / (game->img.size.x - 1));
	game->col_center = (float)game->img.size.x / 2;
	game->col_scale = 1 / game->col_step;
	printf("Real FOV: %.0f\n", 114 * atan(game->col_step * game->col_center));
	if (reset == true || (M_PI_4 / 4 < fov && fov < PI2))
		game->fov = fov;
}

int	game_loop(t_game *game)
{
	player_control(game);
	ray_cast(game);
	img_ceilfloor_fill_rgb(&game->img, game->color_ceil, game->color_floor);
	draw_walls(game);
	draw_objects(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.ptr, 0, 0);
	return (0);
}

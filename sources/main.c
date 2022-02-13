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

int	main(int argc, char *argv[])
{
	t_game	game;

	errno = 0;
	game = (t_game){0};
	if (argc < 2)
		terminate(&game, ERR_ARGS, "Please specify scene filename");
	else if (argc > 2)
		terminate(&game, ERR_ARGS, "Too many arguments");
	game.mlx = mlx_init();
	if (game.mlx == NULL)
		terminate(&game, ERR_MLX, strerror(errno));
	parse(argv[1], &game);
	initialize_game(&game);
	mlx_do_key_autorepeatoff(game.mlx);
	mlx_hook(game.win, KeyPress, KeyPressMask, hook_key_press, &game);
	mlx_hook(game.win, KeyRelease, KeyReleaseMask, hook_key_release, &game);
	mlx_hook(game.win, DestroyNotify, StructureNotifyMask, hook_exit, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_loop(game.mlx);
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
	static clock_t	clock_cur;
	static int		fps;

	player_control(game);
	ray_cast(game);
	img_ceilfloor_fill_rgb(&game->img, game->color_ceil, game->color_floor);
	draw_walls(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.ptr, 0, 0);
	if (clock() != clock_cur)//
		fps = CLOCKS_PER_SEC / (clock() - clock_cur);//
	clock_cur = clock(); //
	mlx_string_put(game->mlx, game->win, 0, 20, 0x00FFFFFF, \
		(char []){'0' + fps / 100, '0' + fps / 10 % 10, '0' + fps % 10, '\0'});
	return (0);
}

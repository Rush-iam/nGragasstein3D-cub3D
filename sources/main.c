/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:58:52 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/05 23:03:20 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "x_events.h"

int	main(void)
{
	t_mlx	mlx;
	int		null;

	if ((mlx.mlx = mlx_init()) == NULL)
		return (EXIT_FAILURE);
	if (!(mlx.win = mlx_new_window(mlx.mlx, WIN_W, WIN_H, "cub3D by nGragas")))
		return (EXIT_FAILURE);
	if (!(mlx.buf.ptr = mlx_new_image(mlx.mlx, WIN_W, WIN_H)))
		return (EXIT_FAILURE);
	mlx.buf.data = (int *)mlx_get_data_addr(mlx.buf.ptr, &null, &null, &null);
	mlx.buf.size = (t_point){WIN_W, WIN_H};
	mlx_do_key_autorepeatoff(mlx.mlx);
	mlx_hook(mlx.win, EVENT_KEYPRESS, 0, hook_key_press, &mlx);
	mlx_hook(mlx.win, EVENT_KEYRELEASE, 0, hook_key_release, &mlx);
	mlx_hook(mlx.win, EVENT_BUTTONPRESS, 0, hook_mouse_press, &mlx);
	mlx_hook(mlx.win, EVENT_BUTTONRELEASE, 0, hook_mouse_release, &mlx);
	mlx_hook(mlx.win, EVENT_MOTIONNOTIFY, 0, hook_mouse_move, &mlx);
	mlx_hook(mlx.win, EVENT_DESTROYNOTIFY, 0, terminate, NULL);
	mlx_loop_hook(mlx.mlx, game_loop, &mlx);
	mlx_loop(mlx.mlx);
}

int	game_loop(t_mlx *mlx)
{
	clock_t	start;
	clock_t	end;
	char	*fps;

	start = clock();

	img_clear(&mlx->buf);
//	img_ceilfloor_fill(&mlx->buf, COLOR_CEIL, COLOR_FLOOR);
//	img_ceilfloor_fill_rgb(&mlx->buf, COLOR_CEIL, COLOR_FLOOR);
//	img_clear_rgb(&mlx->buf, 0x8822FF);
	demo_fillrate(mlx, 1);
//	demo_radar(mlx, 360);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->buf.ptr, 0, 0);

	end = clock();

//	ft_putnbr_fd(CLOCKS_PER_SEC / (end - start), 1);
//	write(1, "\n", 1);
	fps = ft_itoa(CLOCKS_PER_SEC / (end - start));
	mlx_string_put(mlx->mlx, mlx->win, 0, 0, 0xFFFFFF, fps);
	free(fps);
	return (0);
}

int	terminate(void)
{
	exit(EXIT_SUCCESS);
}
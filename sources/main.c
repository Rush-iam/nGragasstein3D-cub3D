/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:58:52 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/03 23:15:20 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	key_hook(int key, t_mlx *mlx)
{
	char	*num1;

	printf("keyboard = %d\n", key);
//	if (key == 53) # escape
//		mlx_destroy_window(mlx->mlx, mlx->win);
	num1 = ft_itoa(key);
	mlx_clear_window(mlx->mlx, mlx->win);
	mlx_string_put(mlx->mlx, mlx->win, 30, 30, 0xFFFFFF, num1);
	free(num1);
	return (0);
}

void	ft_swap(int *x, int *y)
{
	const int	tmp = *x;

	*x = *y;
	*y = tmp;
}

void	ft_swap_points(t_point *p1, t_point *p2)
{
	const t_point	tmp = {p1->x, p1->y};

	p1->x = p2->x;
	p1->y = p2->y;
	p2->x = tmp.x;
	p2->y = tmp.y;
}

void	ft_swap_points_xy(t_point *p1, t_point *p2)
{
	const t_point	tmp = {p1->x, p1->y};

	p1->x = p2->y;
	p1->y = p2->x;
	p2->x = tmp.y;
	p2->y = tmp.x;
}

void	draw_line(t_point p1, t_point p2, int cl, t_mlx *m)
{
	const t_point	d = {abs(p2.x - p1.x), abs(p2.y - p1.y)};
	const int		reverse = d.x < d.y;
	int				error;
	int				cur;
	int				dir;

	if (reverse)
	{
		ft_swap((int *)&d.x, (int *)&d.y);
		ft_swap_points_xy(&p1, &p2);
	}
	(p2.x < p1.x) ? ft_swap_points(&p1, &p2) : 0;
	cur = p1.y;
	error = 0;
	dir = (p2.y > p1.y) - (p2.y < p1.y);
	while (p1.x < p2.x)
	{
		reverse ? mlx_pixel_put(m->mlx, m->win, cur % WIN_W, p1.x++ % WIN_H, cl)
			: mlx_pixel_put(m->mlx, m->win, p1.x++ % WIN_W, cur % WIN_H, cl);
		if ((error += d.y + 1) >= d.x + 1)
		{
			cur += dir;
			error -= d.x + 1;
		}
	}
}

int	game_loop(t_mlx *mlx)
{
	static clock_t	start;
	static clock_t	end;
	static char		*fps;
//	static short	col;
//	int				i;

	start = clock();
	mlx_clear_window(mlx->mlx, mlx->win);
//	i = 0;
//	while ((i += 24) < WIN_W * WIN_H)
//		mlx_pixel_put(mlx->mlx, mlx->win, i % WIN_W, (i / WIN_W) % WIN_H, 0xFF);
//
//#define P1 WIN_W / 2, WIN_H / 2
//#define P2 WIN_W / 2 + 300 * cos((col * 2 * M_PI) / 2048), WIN_H / 2 + 300 * sin((col * 2 * M_PI) / 2048)
//	i = 0;
//	while (i++ < 200)
//		{draw_line((t_point){P1}, (t_point){P2}, 0xAFAF, mlx); col++;}
	end = clock();

	fps = ft_itoa(CLOCKS_PER_SEC / (end - start));
	mlx_string_put(mlx->mlx, mlx->win, 0, 0, 0xFFFFFF, fps);
	free(fps);
	return (0);
}

int mouse_hook(int button, int x, int y, t_mlx *mlx)
{
	(void)mlx;
	printf("button = %d, x = %d, y = %d\n", button, x, y);
	return (0);
}

int main()
{
	t_mlx	mlx;

	if ((mlx.mlx = mlx_init()) == NULL)
		return (1);
	if (!(mlx.win = mlx_new_window(mlx.mlx, WIN_W, WIN_H, "cub3D")))
		return (1);
//	mlx_pixel_put(mlx.mlx, mlx.win, 251, 250, 0xFF0000);
//	mlx_pixel_put(mlx.mlx, mlx.win, 250, 250, 0xFFFFFF);
	mlx_key_hook(mlx.win, key_hook, &mlx);
	mlx_mouse_hook(mlx.win, mouse_hook, &mlx);
	mlx_loop_hook(mlx.mlx, game_loop, &mlx);
	mlx_loop(mlx.mlx);
	printf("Hello, World!\n");
	return (0);
}

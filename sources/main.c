/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:58:52 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/04 22:56:02 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	key_hook(int key, t_mlx *mlx)
{
	char	*num1;

	ft_putstr_fd("\nkeyboard = ", 1);
	ft_putnbr_fd(key, 1);
	if (key == K_ESCAPE)
		exit(0);
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

void	pixel_put(t_img *img, int x, int y, int color)
{
	if (x < 0 || x >= img->size.x || y < 0 || y >= img->size.y)
		return ;
	img->data[y * img->size.x + x] = color;
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
		reverse ? pixel_put(&m->buf, cur, p1.x++, cl)
			: pixel_put(&m->buf, p1.x++, cur, cl);
		if ((error += d.y + 1) >= d.x + 1)
		{
			cur += dir;
			error -= d.x + 1;
		}
	}
}

void	demo_fillrate(int step, t_mlx *mlx)
{
	short		x;
	short		y;
	static char	shift;

	if (!step)
		return ;
	y = 0;
	while (y < WIN_H)
	{
		x = 0;
		while (x < WIN_W)
		{
			pixel_put(&mlx->buf, x, y, x * y + shift);
			x += step;
		}
		shift++;
		y += step;
	}
}

void	demo_radar(int rays, t_mlx *mlx)
{
	static short	shift;
	const t_point	p1 = {WIN_W / 2, WIN_H / 2};
	t_point			p2;
	int				i;

	i = 0;
	while (i++ < rays)
	{
		p2 = (t_point) {p1.x + WIN_H * cos(((shift + i) * 2 * M_PI) / 360),
						p1.y + WIN_H * sin(((shift + i) * 2 * M_PI) / 360)};
		draw_line(p1, p2, 0xAFAF - i, mlx);
	}
	shift += 8;
}

void	img_clear(t_img *img)
{
	ft_bzero(img->data, 4 * img->size.x * img->size.y);
}

int	game_loop(t_mlx *mlx)
{
	clock_t	start;
	clock_t	end;
	char	*fps;

	start = clock();
	img_clear(&mlx->buf);

	demo_fillrate(1, mlx);
//	demo_radar(360, mlx);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->buf.ptr, 0, 0);

	end = clock();

//	ft_putnbr_fd(CLOCKS_PER_SEC / (end - start), 1);
	fps = ft_itoa(CLOCKS_PER_SEC / (end - start));
	mlx_string_put(mlx->mlx, mlx->win, 0, 0, 0xFFFFFF, fps);
	free(fps);
	return (0);
}

//int mouse_hook(int button, int x, int y, t_mlx *mlx)
//{
//	(void)mlx;
//	printf("button = %d, x = %d, y = %d\n", button, x, y);
//	return (0);
//}

int main()
{
	t_mlx	mlx;
	int		tmp;

	if ((mlx.mlx = mlx_init()) == NULL)
		return (1);
	if (!(mlx.win = mlx_new_window(mlx.mlx, WIN_W, WIN_H, "cub3D by nGragas")))
		return (1);
	mlx.buf.ptr = mlx_new_image(mlx.mlx, WIN_W, WIN_H);
	mlx.buf.data = (int *)mlx_get_data_addr(mlx.buf.ptr, &tmp, &tmp, &tmp);
	mlx.buf.size = (t_point){WIN_W, WIN_H};
	mlx_key_hook(mlx.win, key_hook, &mlx);
//	mlx_mouse_hook(mlx.win, mouse_hook, &mlx);
	mlx_loop_hook(mlx.mlx, game_loop, &mlx);
	mlx_loop(mlx.mlx);
	return (0);
}

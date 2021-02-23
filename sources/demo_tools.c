/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 19:39:00 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/23 20:04:14 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	demo_fillrate(t_game *mlx, int step)
{
	unsigned short	x;
	unsigned short	y;
	static char		shift;

	if (!step)
		return ;
	y = 0;
	while (y < mlx->img.size.y)
	{
		x = 0;
		while (x < mlx->img.size.x)
		{
			pixel_put(&mlx->img, x, y, x * y + shift);
			x += step;
		}
		shift++;
		y += step;
	}
}

void	demo_radar(t_game *mlx, int rays)
{
	static short	shift;
	const t_point	p1 = {mlx->img.size.x / 2, mlx->img.size.y / 2};
	t_point			p2;
	int				i;

	i = 0;
	while (i++ < rays)
	{
		p2 = (t_point) {p1.x + mlx->img.size.y * cos(((shift + i) * 2 * M_PI) / 360),
					p1.y + mlx->img.size.y * sin(((shift + i) * 2 * M_PI) / 360)};
		draw_line(&mlx->img, p1, p2, 0xAFAF - i);
	}
	shift += 8;
}

void	demo_cursor(t_game *game, int color)
{
	draw_line(&game->img, game->key.mpos,
			points_sum(game->key.mpos, game->key.mdir), color);
}

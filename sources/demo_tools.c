/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 19:39:00 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/06 16:44:32 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	demo_fillrate(t_game *mlx, int step)
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
	const t_point	p1 = {WIN_W / 2, WIN_H / 2};
	t_point			p2;
	int				i;

	i = 0;
	while (i++ < rays)
	{
		p2 = (t_point) {p1.x + WIN_H * cos(((shift + i) * 2 * M_PI) / 360),
						p1.y + WIN_H * sin(((shift + i) * 2 * M_PI) / 360)};
		draw_line(&mlx->img, p1, p2, 0xAFAF - i);
	}
	shift += 8;
}

void	demo_cursor(t_game *game, int color)
{
	draw_line(&game->img, game->key.mpos,
			  points_sum(game->key.mpos, game->key.mdir), color);
}
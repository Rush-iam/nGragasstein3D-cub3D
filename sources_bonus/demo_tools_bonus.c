/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   demo_tools_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:30:09 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/06 17:30:09 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

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
			mlx->img.data[y * mlx->img.size.x + x] = x * y + shift;
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

/*
**void	demo_cursor(t_game *game, int color)
**{
**	draw_line(&game->img, game->key.mpos,
**		(t_point){game->key.mpos.x + game->key.mdir.x,
**			game->key.mpos.y + game->key.mdir.y}, color);
**}
*/

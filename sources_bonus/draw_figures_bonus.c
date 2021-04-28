/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_figures_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:09 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/05 19:22:37 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_line(t_img *img, t_point p1, t_point p2, int color)
{
	const t_point	diff = {abs(p2.x - p1.x), -abs(p2.y - p1.y)};
	const int		step_x = (p1.x < p2.x) - (p1.x >= p2.x);
	const int		step_y = (p1.y < p2.y) - (p1.y >= p2.y);
	int				error;
	int				error_2;

	error = diff.x + diff.y;
	while (true)
	{
		pixel_put(img, p1.x, p1.y, color);
		if (p1.x == p2.x && p1.y == p2.y)
			break ;
		error_2 = 2 * error;
		if (error_2 >= diff.y)
		{
			error += diff.y;
			p1.x += step_x;
		}
		if (error_2 <= diff.x)
		{
			error += diff.x;
			p1.y += step_y;
		}
	}
}

void	draw_4pts(t_img *img, t_point *pts, int color)
{
	draw_line(img, pts[0], pts[1], color);
	draw_line(img, pts[1], pts[2], color);
	draw_line(img, pts[2], pts[3], color);
	draw_line(img, pts[3], pts[0], color);
}

void	draw_square(t_img *img, t_point center, int size, int color)
{
	const int	shift = size / 2;

	center.x -= shift;
	center.y -= shift;
	draw_line(img, center, (t_point){center.x + size, center.y}, color);
	draw_line(img, (t_point){center.x + size, center.y},
					(t_point){center.x + size, center.y + size}, color);
	draw_line(img, (t_point){center.x + size, center.y + size},
					(t_point){center.x, center.y + size}, color);
	draw_line(img, (t_point){center.x, center.y + size}, center, color);
}

void	draw_square_fill(t_img *img, t_point top_left, int size, int color)
{
	t_point		px;

	px.y = top_left.y;
	while (px.y < top_left.y + size)
	{
		px.x = top_left.x;
		while (px.x < top_left.x + size)
		{
			pixel_put(img, px.x, px.y, color);
			px.x++;
		}
		px.y++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_figures.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 19:33:42 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/05 21:19:02 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	pixel_put(t_img *img, int x, int y, int color)
{
	if (x < 0 || x >= img->size.x || y < 0 || y >= img->size.y)
		return ;
	img->data[y * img->size.x + x] = color;
}

void	draw_line(t_img *img, t_point p1, t_point p2, int color)
{
	const t_point	diff = {abs(p2.x - p1.x), -abs(p2.y - p1.y)};
	const int		step_x = (p1.x < p2.x) - (p1.x >= p2.x);
	const int		step_y = (p1.y < p2.y) - (p1.y >= p2.y);
	int				error;
	int				error_2;

	error = diff.x + diff.y;
	while (1)
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

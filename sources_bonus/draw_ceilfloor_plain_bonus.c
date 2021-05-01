/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ceilfloor_plain_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 00:21:47 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/01 14:33:45 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	ceilfloor_plain_generate(t_img *img, unsigned ceil, unsigned floor, unsigned fade_distance)
{
	t_upoint	cur;
	int			line_color;

	cur.y = 0;
	while (cur.y < img->size.y)
	{
		if (cur.y < img->size.y / 2)
			line_color = pixel_fade(ceil, 1 - powf(
					cur.y / (img->size.y / 2.f) / powf(fade_distance, 0.25f), 2.f));
		else
			line_color = pixel_fade(floor, 1 - powf(
					(2 - 2.f * cur.y / img->size.y) / powf(fade_distance, 0.25f), 2.f));
		cur.x = 0;
		while (cur.x < img->size.x)
			img->data[cur.y * img->size.x + cur.x++] = line_color;
		cur.y++;
	}
}

void	draw_ceil_plain(t_game *g)
{
	const int	pixel_offset = (g->horizon - g->center.y) * (int)g->img.size.x;
	int			i;

	if (pixel_offset > 0)
	{
		i = ft_min(g->img_pixelcount, pixel_offset);
		while (i >= 0)
			g->img.data[i--] = g->color_ceil;
		if (pixel_offset < g->img_pixelcount)
			ft_memcpy(g->img.data + pixel_offset, g->img_bg.data,
				(ft_min(g->img_pixelcount / 2,
							  g->img_pixelcount - pixel_offset)) * 4);
	}
	else if (-pixel_offset < g->img_pixelcount / 2)
		ft_memcpy(g->img.data, g->img_bg.data - pixel_offset,
				  (g->img_pixelcount / 2 + pixel_offset) * 4);
}

void	draw_floor_plain(t_game *g)
{
	const int	pixel_offset = (g->horizon - g->center.y) * (int)g->img.size.x;
	int			i;
	const int	shift = ft_max(0, -(g->img_pixelcount / 2 + pixel_offset));

	if (pixel_offset < 0)
	{
		if (-pixel_offset < g->img_pixelcount)
			ft_memcpy(g->img.data +
					  ft_max(g->img_pixelcount / 2 + pixel_offset, 0),
					  g->img_bg.data + g->img_pixelcount / 2 + shift,
					  (g->img_pixelcount / 2 - shift) * 4);
		i = ft_max(0, g->img_pixelcount + pixel_offset);
		while (i < g->img_pixelcount)
			g->img.data[i++] = g->color_floor;
	}
	else if (pixel_offset < g->img_pixelcount / 2)
		ft_memcpy(g->img.data + g->img_pixelcount / 2 + pixel_offset,
				  g->img_bg.data + g->img_pixelcount / 2,
				  (g->img_pixelcount / 2 - pixel_offset) * 4);
}

void	draw_ceilfloor_plain(t_game *g)
{
	const int	pixel_offset = (g->horizon - g->center.y) * (int)g->img.size.x;
	int			i;

	if (pixel_offset < 0)
	{
		if (-pixel_offset < g->img_pixelcount)
			ft_memcpy(g->img.data, g->img_bg.data - pixel_offset,
					  (g->img_pixelcount + pixel_offset) * 4);
		i = ft_max(0, g->img_pixelcount + pixel_offset);
		while (i < g->img_pixelcount)
			g->img.data[i++] = g->color_floor;
	}
	else
	{
		i = ft_min(g->img_pixelcount, pixel_offset);
		while (i >= 0)
			g->img.data[i--] = g->color_ceil;
		if (pixel_offset < g->img_pixelcount)
			ft_memcpy(g->img.data + pixel_offset, g->img_bg.data,
					  (g->img_pixelcount - pixel_offset) * 4);
	}
}

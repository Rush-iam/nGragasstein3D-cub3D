/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_pixels_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 15:51:31 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/28 15:51:31 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	pixel_put(t_img *img, unsigned x, unsigned y, int color)
{
	if (x >= img->size.x || y >= img->size.y)
		return ;
	img->data[y * img->size.x + x] = color;
}

int		pixel_fade(int color, float fade)
{
//	if (fade < 0.5f)
//		return (color >> 1 & 0x7F7F7F);
//	else
	return (((int)((color & 0xFF0000) * fade) & 0xFF0000) |
			((int)((color & 0xFF00) * fade) & 0xFF00) |
			(int)((color & 0xFF) * fade));
}

int		pixel_alpha(int color, float alpha)
{
	return ((int)(alpha * 255) << 24 | color);
}

int		pixel_fade_contrast(int color, float fade)
{
	int r, g, b;
	int res;

	r = (color & 0xFF0000) >> 16;
	g = (color & 0xFF00) >> 8;
	b = color & 0xFF;
	if (r > fade * 255)
		r = r - fade * (255 - r);
	else
		r = r * sqrt(fade);
	if (g > fade * 255)
		g = g - fade * (255 - g);
	else
		g = g * sqrt(fade);
	if (b > fade * 255)
		b = b - fade * (255 - b);
	else
		b = b * sqrt(fade);
	res = (r << 16) | (g << 8) | b;
	return (pixel_fade(res, 0.85));
}

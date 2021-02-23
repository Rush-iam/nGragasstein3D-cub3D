/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_fills.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 19:32:14 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/23 21:01:58 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	img_clear(t_img *img)
{
	ft_bzero(img->data, 4 * img->size.x * img->size.y);
}

void	img_clear_rgb(t_img *img, int color)
{
	const int	img_size = img->size.x * img->size.y;
	int			i;

	i = 0;
	while (i < img_size)
		img->data[i++] = color;
}

void	img_ceilfloor_fill(t_img *img, unsigned char ceil, unsigned char floor)
{
	const int	half_screen = img->size.x * img->size.y / 2;
	const int	half_screen_bytes = half_screen * 4;

	ft_memset(img->data, ceil, half_screen_bytes);
	ft_memset(img->data + half_screen, floor, half_screen_bytes);
}

void	img_ceilfloor_fill_rgb(t_img *img, int ceil, int floor)
{
	const int	full_screen = img->size.x * img->size.y;
	const int	half_screen = full_screen / 2;
	int			i;

	i = 0;
	while (i < half_screen)
		img->data[i++] = ceil;
	while (i < full_screen)
		img->data[i++] = floor;
}

void	fizzlefade(t_img *img, int color)
{
	static unsigned	rndval = 1;
	static int		frames;
	unsigned		lsb;
	t_upoint		pos;
	int				i;

	if (frames == 256)
		return ;
	frames++;
	i = 0;
	while (i++ < 512)
	{
		pos.x = ((rndval & 0x1FF00) >> 8) * 2;
		pos.y = (rndval & 0x000FF) * 2;
		lsb = rndval & 1;
		rndval >>= 1;
		if (lsb)
			rndval ^= 0x12000;
		pixel_put(img, pos.x, pos.y, color);
		pixel_put(img, pos.x + 1, pos.y, color);
		pixel_put(img, pos.x, pos.y + 1, color);
		pixel_put(img, pos.x + 1, pos.y + 1, color);
	}
}

/*
**void	img_clear_rgb_128(t_img *img, int color)
**{
**	const long			int64 = color | ((long)color << 32);
**	const __int128_t	int128 = int64 | ((__int128_t)int64 << 64);
**	int					img_size;
**	int					tail;
**	int					i;
**
**	img_size = img->size.x * img->size.y;
**	tail = img_size % 4;
**	img_size -= tail;
**	i = 0;
**	while (i < img_size)
**	{
**		*(__int128_t *)(img->data + i) = int128;
**		i += 4;
**	}
**	while (tail)
**		img->data[i + tail--] = color;
**}
*/

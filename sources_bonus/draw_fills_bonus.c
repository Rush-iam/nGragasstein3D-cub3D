/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_fills_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:18 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/12 15:31:09 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	img_clear(t_img *img)
{
	ft_bzero(img->data, 4 * img->size.x * img->size.y);
}

void	img_clear_rgb(t_img *img, unsigned color)
{
	const int	img_size = img->size.x * img->size.y;
	int			i;

	i = 0;
	while (i < img_size)
		img->data[i++] = color;
}

void	img_ceilfloor_rgb(t_img *img, unsigned ceil, unsigned floor)
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

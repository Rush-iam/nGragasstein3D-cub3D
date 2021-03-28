/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:41 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/28 18:51:39 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

char	*atoi_limited(unsigned *dst_int, const char *src_string, unsigned limit)
{
	unsigned long	num;

	while (*src_string == ' ')
		src_string++;
	if (ft_isdigit(*src_string) == false)
		return (NULL);
	num = 0;
	while (ft_isdigit(*src_string))
	{
		num = num * 10 + *src_string++ - '0';
		if (limit != UINT_MAX && num > limit)
			return (NULL);
		if (num > UINT_MAX)
			while (ft_isdigit(*src_string))
				src_string++;
	}
	*dst_int = ft_umin(num, UINT_MAX);
	while (*src_string == ' ')
		src_string++;
	return ((char *)src_string);
}

t_img	img_resize(void *mlx_ptr, t_img *src_img, t_upoint dstres)
{
	t_img		dst_img;
	int			n;
	t_upoint	src_pix;
	t_upoint	dst_pix;

	if ((dst_img.ptr = mlx_new_image(mlx_ptr, dstres.x, dstres.y)) == NULL)
		return (dst_img);
	dst_img.data = (unsigned *)mlx_get_data_addr(dst_img.ptr, &n, &n, &n);
	dst_img.size = dstres;
	dst_img.aspect = dstres.x / dstres.y;
	dst_pix.y = 0;
	while (dst_pix.y < dst_img.size.y)
	{
		dst_pix.x = 0;
		while (dst_pix.x < dst_img.size.x)
		{
			src_pix = (t_upoint){src_img->size.x * dst_pix.x / dst_img.size.x,
								 src_img->size.y * dst_pix.y / dst_img.size.y};
			dst_img.data[dst_pix.y * dst_img.size.x + dst_pix.x++] =
					src_img->data[src_pix.y * src_img->size.x + src_pix.x];
		}
		dst_pix.y++;
	}
	mlx_destroy_image(mlx_ptr, src_img->ptr);
	free(src_img->alpha_y);
	dst_img.alpha_y = NULL;
	return (dst_img);
}

t_img	img_faded_copy(void *mlx_ptr, t_img *img)
{
	t_img	img_faded;
	int		i;

	img_faded.ptr = mlx_new_image(mlx_ptr, img->size.x, img->size.y);
	if (img_faded.ptr == NULL)
		return (img_faded);
	img_faded.data = (unsigned *)mlx_get_data_addr(img_faded.ptr, &i, &i, &i);
	img_faded.size = img->size;
	img_faded.aspect = img->aspect;
	img_faded.alpha_y = NULL;
	i = 0;
	while (i < (int)(img->size.x * img->size.y))
	{
		img_faded.data[i] = pixel_fade(img->data[i], 0.65);
		i++;
	}
	return (img_faded);
}

void	img_alpha_columns_get(t_img *img)
{
	t_upoint	px;
	bool		alpha_y;

	img->min_x = img->size.x - 1;
	img->max_x = 0;
	px.y = 0;
	while (px.y < img->size.y)
	{
		px.x = 0;
		alpha_y = true;
		while (px.x < img->size.x)
		{
			if ((img->data[px.y * img->size.x + px.x] >> 24) != 0xFF)
			{
				img->min_x = ft_umin(img->min_x, px.x);
				img->max_x = ft_umax(img->max_x, px.x + 1);
				alpha_y = false;
			}
			px.x++;
		}
		img->alpha_y[px.y++] = alpha_y;
	}
	img->min_x = fminf(img->min_x, img->max_x);
	img->min_x = img->min_x / img->size.x;
	img->max_x = img->max_x / img->size.x;
}


void	write_screenshot_and_exit(t_game *game)
{
	int				file_id;
	char			header[26];
	const unsigned	filesize = 26 + 3 * game->img.size.x * game->img.size.y;

	objects(game);
	weapon(game, &game->p.weapon);
	ray_cast(game);
	img_ceilfloor_rgb(&game->img, game->color_ceil, game->color_floor);
	draw_walls(game);
	draw_objects(game);
	if ((file_id = open("shot.bmp", O_WRONLY | O_CREAT | O_TRUNC)) == -1)
		terminate(game, ERR_BMP, strerror(errno));
	ft_memcpy(header, "BM", 2);
	ft_memcpy(header + 2, &filesize, 4);
	ft_memcpy(header + 6, "\0\0\0\0\x1A\0\0\0\x0C\0\0\0", 12);
	ft_memcpy(header + 18, &game->img.size.x, 2);
	ft_memcpy(header + 20, &game->img.size.y, 2);
	ft_memcpy(header + 22, "\1\0\x18\0", 4);
	if (write(file_id, header, 26) == -1)
		terminate(game, ERR_BMP, strerror(errno));
	write_screenshot_data(game, file_id);
	close(file_id);
	terminate(game, 0, NULL);
}

void	write_screenshot_data(t_game *game, int file_id)
{
	char		*bmp_data;
	t_upoint	px;
	unsigned	i;

	if ((bmp_data = malloc(3 * game->img.size.x * game->img.size.y +
			game->img.size.y * ((4 - (game->img.size.x * 3) % 4) % 4))) == NULL)
		terminate(game, ERR_BMP, strerror(errno));
	i = 0;
	px.y = game->img.size.y;
	while (px.y-- > 0)
	{
		px.x = 0;
		while (px.x < game->img.size.x)
		{
			ft_memcpy(bmp_data + i,
						&game->img.data[game->img.size.x * px.y + px.x++], 3);
			i += 3;
		}
		ft_bzero(bmp_data + i, (4 - i % 4) % 4);
		i += (4 - i % 4) % 4;
	}
	if (write(file_id, bmp_data, i) == -1)
		terminate(game, ERR_BMP, strerror(errno));
	free(bmp_data);
}

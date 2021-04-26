/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:41 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/26 22:59:33 by ngragas          ###   ########.fr       */
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

t_img	*img_create(void *mlx_ptr, t_img *dst, t_upoint size)
{
	int n;

	dst->ptr = mlx_new_image(mlx_ptr, size.x, size.y);
	if (dst->ptr == NULL)
		return (NULL);
	dst->data = (void *)mlx_get_data_addr(dst->ptr, &n, &n, &n);
	dst->size = (t_upoint){size.x, size.y};
	dst->aspect = (float)size.x / size.y;
	return (dst);
}

void	img_create_from_file(t_game *g, char *path, t_img *dst_img, char *err)
{
	int				n;
	const size_t	str_len = ft_strlen(path);

	if (str_len < 5)
		terminate(g, ERR_ARGS, "Can't identify texture format (.xpm/.png)");
	if (ft_memcmp(".xpm", path + str_len - 4, 5) == 0)
		dst_img->ptr = mlx_xpm_file_to_image(g->mlx, path,
						(int *)&dst_img->size.x, (int *)&dst_img->size.y);
	else if (ft_memcmp(".png", path + str_len - 4, 5) == 0)
		dst_img->ptr = mlx_png_file_to_image(g->mlx, path,
						(int *)&dst_img->size.x, (int *)&dst_img->size.y);
	else
		terminate(g, ERR_ARGS, "Can't identify texture format (.xpm/.png)");
	if (dst_img->ptr == NULL)
		terminate(g, ERR_PARSE, err);
	dst_img->data = (unsigned *)mlx_get_data_addr(dst_img->ptr, &n, &n, &n);
	dst_img->aspect = dst_img->size.x / dst_img->size.y;
	dst_img->alpha_y = ft_calloc(dst_img->size.y, sizeof(bool));
	if (dst_img->alpha_y == NULL)
		terminate(g, ERR_MEM, strerror(errno));
	img_alpha_columns_get(dst_img);
}

t_img	*img_resize(void *mlx_ptr, t_img *src_img, t_upoint dstres)
{
	t_img		dst_img;
	t_upoint	src_pix;
	t_upoint	dst_pix;

	if (img_create(mlx_ptr, &dst_img, dstres) == NULL)
		return (NULL);
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
	dst_img.alpha_y = NULL;
	free(src_img->alpha_y);
	mlx_destroy_image(mlx_ptr, src_img->ptr);
	*src_img = dst_img;
	return (src_img);
}

t_img	img_faded_copy(void *mlx_ptr, t_img *img)
{
	t_img	img_faded;
	int		i;

	if (img_create(mlx_ptr, &img_faded, img->size) == NULL)
		return (img_faded);
	img_faded.alpha_y = NULL;
	i = 0;
	while (i < (int)(img->size.x * img->size.y))
	{
		img_faded.data[i] = pixel_fade(img->data[i], 0.65f);
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

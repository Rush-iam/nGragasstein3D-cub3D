/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 15:13:44 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 15:25:52 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

t_img	*img_create(void *mlx_ptr, t_img *dst, t_upoint size)
{
	int	null;

	dst->ptr = new_image(mlx_ptr, size.x, size.y);
	if (dst->ptr == NULL)
		return (NULL);
	dst->data = (void *)mlx_get_data_addr(dst->ptr, &null, &null, &null);
	dst->size = (t_upoint){size.x, size.y};
	dst->aspect = (float)size.x / size.y;
	return (dst);
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
			src_pix = (t_upoint){src_img->size.x * dst_pix.x / dst_img.size.x, \
								src_img->size.y * dst_pix.y / dst_img.size.y};
			dst_img.data[dst_pix.y * dst_img.size.x + dst_pix.x++] = \
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

void	img_from_file_scaled(t_game *g, char *path, t_img *dst_img, float scale)
{
	load_image_file(g, path, dst_img, "Error loading HUD image file");
	if (img_resize(g->mlx, dst_img, (t_upoint){dst_img->size.x * scale, \
											  dst_img->size.y * scale}) == NULL)
		terminate(g, ERR_MEM, strerror(errno));
}

// float to int?
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
			if ((img->data[px.y * img->size.x + px.x] >> 24) == ALPHA_OPAQUE)
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 16:20:11 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/01 23:36:35 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	*atoi_limited(unsigned *dst_int, const char *src_string, unsigned limit)
{
	while (*src_string == ' ')
		src_string++;
	if (ft_isdigit(*src_string) == false)
		return (NULL);
	while (ft_isdigit(*src_string))
	{
		*dst_int = *dst_int * 10 + *src_string++ - '0';
		if (*dst_int > limit)
			return (NULL);
	}
	while (*src_string == ' ')
		src_string++;
	return ((char *)src_string);
}

int		terminate(int return_value, char *message)
{
	if (return_value)
	{
		ft_putendl_fd("Error", 2);
		ft_putendl_fd(message, 2);
		if (return_value == ERROR_MLX)
			ft_putendl_fd("MLX/X11 crashed", 2);
		else if (return_value == ERROR_PARSE)
			ft_putendl_fd("Plese fix scene file", 2);
		else if (return_value == ERROR_ARGS)
			ft_putendl_fd("Usage: ./cub3D scene_name.cub [--save]", 2);
		else if (return_value == ERROR_BMP)
			ft_putendl_fd("Unable to save screenshot", 2);
	}
	exit(return_value); //
}

void	write_screenshot_and_exit(t_game *game)
{
	int				file_id;
	char			header[26];
	const unsigned	filesize = 26 + 3 * game->img.size.x * game->img.size.y;

	img_ceilfloor_fill_rgb(&game->img, game->color_ceil, game->color_floor);
	ray_cast(game);
	draw_walls(game);
	draw_objects(game);
	if ((file_id = open("shot.bmp", O_WRONLY | O_CREAT | O_TRUNC)) == -1)
		terminate(ERROR_BMP, strerror(errno));
	ft_memcpy(header, "BM", 2);
	ft_memcpy(header + 2, &filesize, 4);
	ft_memcpy(header + 6, "\0\0\0\0\x1A\0\0\0\x0C\0\0\0", 12);
	ft_memcpy(header + 18, &game->img.size.x, 2);
	ft_memcpy(header + 20, &game->img.size.y, 2);
	ft_memcpy(header + 22, "\1\0\x18\0", 4);
	if (write(file_id, header, 26) == -1)
		terminate(ERROR_BMP, strerror(errno));
	write_screenshot_and_exit_2(&game->img, file_id);
}

void	write_screenshot_and_exit_2(t_img *img, int file_id)
{
	char		*bmp_data;
	t_upoint	px;
	unsigned	i;

	if ((bmp_data = malloc(3 * img->size.x * img->size.y +
				img->size.y * ((4 - (img->size.x * 3) % 4) % 4))) == NULL)
		terminate(ERROR_BMP, strerror(errno));
	i = 0;
	px.y = img->size.y;
	while (px.y-- > 0)
	{
		px.x = 0;
		while (px.x < img->size.x)
		{
			ft_memcpy(bmp_data + i, &img->data[img->size.x * px.y + px.x++], 3);
			i += 3;
		}
		ft_bzero(bmp_data + i, (4 - i % 4) % 4);
		i += (4 - i % 4) % 4;
	}
	if (i > INT_MAX - 5000)
	{
		write(file_id, bmp_data, INT_MAX - 5000);
		write(file_id, bmp_data + INT_MAX - 5000, i - (INT_MAX - 5000));
	}
	else if (write(file_id, bmp_data, i) == -1)
		terminate(ERROR_BMP, strerror(errno));
	close(file_id);
	exit(0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:41 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/08 23:28:56 by ngragas          ###   ########.fr       */
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
	*dst_int = (num > UINT_MAX) ? UINT_MAX : num;
	while (*src_string == ' ')
		src_string++;
	return ((char *)src_string);
}

void	write_screenshot_and_exit(t_game *game)
{
	int				file_id;
	char			header[26];
	const unsigned	filesize = 26 + 3 * game->img.size.x * game->img.size.y;

	img_ceilfloor_fill_rgb(&game->img, game->color_ceil, game->color_floor);
	ray_cast(game);
	draw_walls(game);
	objects(game);
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

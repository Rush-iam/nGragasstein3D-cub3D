/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screenshot_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 14:27:17 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/02 20:31:26 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

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
	if ((file_id = open("shot.bmp", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU)) == -1)
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

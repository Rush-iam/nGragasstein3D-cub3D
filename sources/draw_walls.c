/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 18:37:14 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/02 19:56:15 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

void	draw_walls(t_game *game)
{
	unsigned int	ray;
	unsigned int	texture_id;
	const char		*textures = "NSWE";

	ray = 0;
	while (ray < game->img.size.x)
	{
		texture_id = ft_strchr(textures, game->column[ray].dir) - textures;
		draw_wall_scaled(game, &game->texture[texture_id], ray);
		ray++;
	}
}

void	draw_wall_scaled(t_game *game, t_img *src, unsigned int x)
{
	const double		step = (double)src->size.y / game->column[x].height;
	const unsigned int	src_x = game->column[x].texture_pos * src->size.x;
	double				src_y;
	int					y;
	int					max_height;

	if (game->column[x].height > game->img.size.y)
	{
		src_y = step * (game->column[x].height - game->img.size.y) / 2;
		y = -1;
		max_height = game->img.size.y;
	}
	else
	{
		src_y = 0;
		y = -1 + (game->img.size.y - game->column[x].height) / 2;
		max_height = y + game->column[x].height;
	}
	while (++y < max_height)
	{
		game->img.data[y * game->img.size.x + x] = \
				src->data[(unsigned)src_y * src->size.x + src_x];
		src_y += step;
	}
}

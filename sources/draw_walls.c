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

void	draw_wall_scaled_new(t_img *game_img, const t_img *src_img, \
							const struct s_column *column, unsigned int x)
{
	const unsigned int	src_x = column->texture_pos * src_img->size.x;
	unsigned int		src_y;
	unsigned int		dst_y;
	unsigned int		max_y;
	unsigned int		error;

	src_y = 0;
	dst_y = ft_max(0, ((int)game_img->size.y - (int)column->height) / 2);
	max_y = dst_y + column->height;
	error = src_img->size.y / 2;
	if (column->height > game_img->size.y)
	{
		error = (column->height - game_img->size.y) * src_img->size.y / 2;
		src_y = error / column->height;
		error = error % column->height;
		max_y = game_img->size.y;
	}
	while (dst_y < max_y)
	{
		while (error >= column->height && ++src_y)
			error -= column->height;
		game_img->data[game_img->size.x * dst_y++ + x] = \
			src_img->data[src_y * src_img->size.x + src_x];
		error += src_img->size.y;
	}
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
		if (game->test)
			draw_wall_scaled_new(\
			&game->img, &game->texture[texture_id], &game->column[ray], ray);
		else
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

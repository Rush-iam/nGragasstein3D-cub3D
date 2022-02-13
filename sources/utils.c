/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 16:20:11 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/02 20:37:42 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	*atoi_limited(unsigned int *dst_int, const char *src_string, \
															unsigned int limit)
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
	*dst_int = ft_umin(UINT_MAX, num);
	while (*src_string == ' ')
		src_string++;
	return ((char *)src_string);
}

int	terminate(t_game *game, int return_value, char *message)
{
	if (return_value)
	{
		ft_putendl_fd("Error", 2);
		ft_putendl_fd(message, 2);
		if (return_value == ERR_MLX)
			ft_putendl_fd("MLX/X11 crashed", 2);
		else if (return_value == ERR_PARSE)
			ft_putendl_fd("Plese fix scene file", 2);
		else if (return_value == ERR_ARGS)
			ft_putendl_fd("Usage: ./cub3D scene_name.cub [--save]", 2);
		else if (return_value == ERR_BMP)
			ft_putendl_fd("Unable to save screenshot", 2);
	}
	free_resources(game);
	if (game->img.ptr)
		mlx_destroy_image(game->mlx, game->img.ptr);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	free(game->mlx);
	exit(return_value);
}

void	free_resources(t_game *g)
{
	unsigned int	i;

	free(g->column);
	if (g->map.grid)
	{
		i = 0;
		while (i < g->map.size.y)
			free(g->map.grid[i++]);
		free(g->map.grid);
	}
	i = 0;
	while (i < sizeof(g->texture) / sizeof(t_img))
	{
		if (g->texture[i].ptr)
			mlx_destroy_image(g->mlx, g->texture[i].ptr);
		i++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 23:31:40 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/08 23:31:40 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int		terminate(t_game *game, int return_value, char *message)
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
	terminate_free(game);
	if (game->map.img.ptr)
		mlx_destroy_image(game->mlx, game->map.img.ptr);
	if (game->img.ptr)
		mlx_destroy_image(game->mlx, game->img.ptr);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	free(game->mlx);
	exit(return_value);
}

void	terminate_free(t_game *game)
{
	unsigned	i;

	ft_lstclear(&game->objects, terminate_free_object);
	if (game->column && (i = 0) == 0)
		while (i < game->img.size.x)
			free(game->column[i++]);
	free(game->column);
	if (game->map.grid && (i = 0) == 0)
		while (i < game->map.size.y)
			free(game->map.grid[i++]);
	free(game->map.grid);
	i = 0;
	while (i < sizeof(game->texture) / sizeof(*game->texture))
	{
		if (game->texture[i].ptr)
			mlx_destroy_image(game->mlx, game->texture[i].ptr);
		i++;
	}
	i = 0;
	while (i < sizeof(game->sprite) / sizeof(*game->sprite))
	{
		if (game->sprite[i].ptr)
			mlx_destroy_image(game->mlx, game->sprite[i].ptr);
		i++;
	}
}

void	terminate_free_object(void *object)
{
	free(((t_object *)object)->e);
	free(object);
}

void	void_function(void *ptr)
{
	(void)ptr;
}

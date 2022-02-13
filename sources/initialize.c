/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 20:55:59 by ngragas           #+#    #+#             */
/*   Updated: 2022/02/11 21:55:11 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef __APPLE__

static inline void	get_screen_size(void *mlx_ptr, int *size_x, int *size_y)
{
	(void)mlx_ptr;
	mlx_get_screen_size(size_x, size_y);
}

#else

static inline void	get_screen_size(void *mlx_ptr, int *size_x, int *size_y)
{
	mlx_get_screen_size(mlx_ptr, size_x, size_y);
}

#endif

void	initialize_game(t_game *g)
{
	int	n;

	get_screen_size(g->mlx, (int *)&g->img.size.x, (int *)&g->img.size.y);
	g->img.aspect = (double)g->img.size.x / g->img.size.y;
	g->win = mlx_new_window(g->mlx, g->img.size.x, g->img.size.y, TITLE);
	if (g->win == NULL)
		terminate(g, ERR_MLX, strerror(errno));
	g->img.ptr = mlx_new_image(g->mlx, g->img.size.x, g->img.size.y);
	if (g->img.ptr == NULL)
		terminate(g, ERR_MLX, strerror(errno));
	g->img.data = (unsigned int *)mlx_get_data_addr(g->img.ptr, &n, &n, &n);
	g->column = malloc(g->img.size.x * sizeof(*g->column));
	if (g->column == NULL)
		terminate(g, ERR_MEM, "Memory allocation failed (ray columns)");
	g->p.vect.y = sin(g->p.angle);
	g->p.vect.x = cos(g->p.angle);
	player_set_fov(g, 0, true);
}

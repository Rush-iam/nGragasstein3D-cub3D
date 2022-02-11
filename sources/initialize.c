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

void	initialize_game(t_game *g, bool screenshot)
{
	t_upoint	max_res;
	int			n;

	if (screenshot)
		max_res = (t_upoint){MAX_SCREENSHOT_X, MAX_SCREENSHOT_Y};
	else
		get_screen_size(g->mlx, (int *)&max_res.x, (int *)&max_res.y);
	g->img.size.x = ft_umax(MIN_RES, g->img.size.x);
	g->img.size.x = ft_umin(max_res.x, g->img.size.x);
	g->img.size.y = ft_umin(max_res.y, g->img.size.y);
	g->img.aspect = (double)g->img.size.x / g->img.size.y;
	if (screenshot == false)
		g->win = mlx_new_window(g->mlx, g->img.size.x, g->img.size.y, TITLE);
	if (screenshot == false && g->win == NULL)
		terminate(g, ERR_MLX, strerror(errno));
	g->img.ptr = mlx_new_image(g->mlx, g->img.size.x, g->img.size.y);
	if (g->img.ptr == NULL)
		terminate(g, ERR_MLX, strerror(errno));
	g->img.data = (unsigned int *)mlx_get_data_addr(g->img.ptr, &n, &n, &n);
	g->column = malloc(g->img.size.x * sizeof(*g->column));
	if (g->column == NULL)
		terminate(g, ERR_MEM, "Memory allocation failed (ray columns)");
	initialize_game_2(g);
}

void	initialize_game_2(t_game *game)
{
	t_list		*cur_list;
	t_object	*obj;

	game->p.vect.y = sin(game->p.angle);
	game->p.vect.x = cos(game->p.angle);
	player_set_fov(game, 0, true);
	cur_list = game->objects;
	while (cur_list)
	{
		obj = (t_object *)cur_list->content;
		obj->distance = game->p.vect.x * (obj->pos.x - game->p.pos.x) + \
						game->p.vect.y * (obj->pos.y - game->p.pos.y);
		cur_list = cur_list->next;
	}
}

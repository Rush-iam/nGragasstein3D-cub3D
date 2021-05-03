/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pahtfind_bfs_grid_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 17:27:46 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 17:27:46 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	bfs_grid_initialize(t_game *g)
{
	uint32_t	y;
	uint32_t	x;

	g->map.grid_bfs = malloc(sizeof(*g->map.grid_bfs) * g->map.size.y);
	if (g->map.grid_bfs == NULL)
		terminate(g, ERR_MEM, "Memory allocation failed (bfs rows)");
	y = 0;
	while (y < g->map.size.y)
	{
		g->map.grid_bfs[y] = malloc(sizeof(**g->map.grid_bfs) * g->map.size.x);
		if (g->map.grid_bfs[y] == NULL)
			terminate(g, ERR_MEM, "Memory allocation failed (bfs cols)");
		x = 0;
		while (x < g->map.size.x)
		{
			if (g->map.grid[y][x] != '.' &&
				ft_strchr(CHAR_DOORS, g->map.grid[y][x]) == NULL)
				g->map.grid_bfs[y][x] = -1U;
			else
				g->map.grid_bfs[y][x] = 0;
			x++;
		}
		y++;
	}
}

void	bfs_grid_update(t_game *g)
{
	uint32_t	y;
	uint32_t	x;

	y = 0;
	while (y < g->map.size.y)
	{
		x = 0;
		while (x < g->map.size.x)
		{
			if (g->map.grid[y][x] != '.' && ft_memchr(CHAR_DOORS, \
					g->map.grid[y][x], sizeof(CHAR_DOORS) - 1) == NULL)
				g->map.grid_bfs[y][x] = -1U;
			else
				g->map.grid_bfs[y][x] = 0;
			x++;
		}
		y++;
	}
}

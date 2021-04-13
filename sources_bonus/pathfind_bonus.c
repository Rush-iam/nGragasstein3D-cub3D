/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfind_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 22:01:43 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/13 22:37:32 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	initialize_bfs_grid(t_game *g)
{
	unsigned	y;
	unsigned	x;

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
			x++;
		}
		y++;
	}
}

bool		pathfind_init(t_game *g, t_object *obj, t_list **queue, t_upoint **pt)
{
	unsigned	y;
	unsigned	x;

	if (obj->e->path != NULL)
		free(obj->e->path);
	*pt = malloc(sizeof(*pt));
	if (*pt == NULL)
		return (false);
	**pt = (t_upoint){obj->e->target.x, obj->e->target.y};
	*queue = ft_lstnew(pt);
	if (*queue == NULL)
		return (false);
	y = 0;
	while (y < g->map.size.y)
	{
		x = 0;
		while (x < g->map.size.x)
		{
			if (g->map.grid_bfs[y][x] != -1U)
				g->map.grid_bfs[y][x] = 0;
			x++;
		}
		y++;
	}
	return (true);
}

t_upoint	*pathfind(t_game *g, t_object *obj)
{
	t_list			*queue;
	t_upoint		*pt;
	t_upoint		*res;
//	const t_upoint	dirs[] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0},
//								{-1, 1}, {0, -1}, {1, 1}};
//	t_upoint		check;

	if (pathfind_init(g, obj, &queue, &pt) == false)
		return (NULL);
	res = NULL;
	while (queue)
	{
		pt = ft_lstpop(&queue);
	}
	return (res);
}

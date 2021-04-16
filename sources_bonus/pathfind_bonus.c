/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfind_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 22:01:43 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/16 18:17:40 by ngragas          ###   ########.fr       */
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
			else
				g->map.grid_bfs[y][x] = 0;
			x++;
		}
		y++;
	}
}

void	*pathfind_deinit(t_list **queue, t_point *pt)
{
	ft_lstclear(queue, free);
	free(pt);
	return (NULL);
}

bool	pathfind_init(struct s_map *map, t_point target, t_list **queue, t_point **pt)
{
	unsigned	y;
	unsigned	x;

	if (ft_malloc_ptr((void **)pt, sizeof(**pt)) == NULL)
		return (NULL);
	**pt = target;
	*queue = ft_lstnew(*pt);
	if (*queue == NULL)
		return (ft_free_ptr(*pt));
	y = 0;
	while (y < map->size.y)
	{
		x = 0;
		while (x < map->size.x)
		{
			if (map->grid_bfs[y][x] != -1U)
				map->grid_bfs[y][x] = 0;
			x++;
		}
		y++;
	}
	map->grid_bfs[target.y][target.x] = 1;
	return (true);
}

int		pathfind_nears(struct s_map *map, t_point pt, t_point *nears)
{
	t_point	check;
	int		i;
	t_point	dirs[] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
	int		counter;

	i = 0;
	counter = 0;
	while (i < 4)
	{
		check = (t_point){pt.x + dirs[i].x, pt.y + dirs[i].y};
		if ((unsigned)check.x < map->size.x &&
				(unsigned)check.y < map->size.y &&
				map->grid_bfs[check.y][check.x] == 0)
			nears[counter++] = check;
		i++;
	}
	return (counter);
}

void	*pathfind_construct(t_list **path, t_point from, struct s_map *map)
{
	t_point		check;
	t_point		*pt;
	t_list		*lst;
	t_point		dirs[] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
	int			i;
	const int	rnd = arc4random() % 4;

	i = 0;
	while (i < 4)
	{
		check = (t_point){from.x + dirs[(i + rnd) % 4].x,
							from.y + dirs[(i + rnd) % 4].y};
		if (map->grid_bfs[check.y][check.x] &&
			map->grid_bfs[check.y][check.x] < map->grid_bfs[from.y][from.x])
		{
			if (ft_malloc_ptr((void *)&pt, sizeof(*pt)) == NULL)
				return (pathfind_deinit(path, NULL));
			*pt = check;
			lst = ft_lstnew(pt);
			if (lst == NULL)
				return (pathfind_deinit(path, pt));
			ft_lstadd_front(path, lst);
			from = check;
			if (map->grid_bfs[from.y][from.x] == 1)
			{
				ft_lstreverse(path);
				return (path);
			}
			return (pathfind_construct(path, from, map));
		}
		i++;
	}
	return (NULL);
}

void	*pathfind(t_list **path, t_point from, t_point to, struct s_map *map)
{
	t_list	*queue;
	t_list	*near_lst;
	t_point	*pt;
	t_point	nears[4];
	int		i;

	ft_lstclear(path, free);
	if (pathfind_init(map, to, &queue, &pt) == false)
		return (NULL);
	while (queue)
	{
		pt = ft_lstpop(&queue);
		i = pathfind_nears(map, *pt, nears);
		while (--i >= 0)
		{
			if (map->grid_bfs[nears[i].y][nears[i].x] == 0)
			{
				map->grid_bfs[nears[i].y][nears[i].x] = map->grid_bfs[pt->y][pt->x] + 1;
				if (nears[i].x == from.x && nears[i].y == from.y)
				{
					pathfind_deinit(&queue, pt);
					return (pathfind_construct(path, from, map));
				}
				near_lst = ft_lstnew(malloc(sizeof(*nears)));
				if (near_lst == NULL)
					return (pathfind_deinit(&queue, pt));
				ft_lstadd_back(&queue, near_lst);
				*((t_point *)near_lst->content) = nears[i];
			}
			else
				map->grid_bfs[nears[i].y][nears[i].x] = ft_umin(
					map->grid_bfs[nears[i].y][nears[i].x], map->grid_bfs[pt->y][pt->x] + 1);
		}
		free(pt);
	}
	return (NULL);
}



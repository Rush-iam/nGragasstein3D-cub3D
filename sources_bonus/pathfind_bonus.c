/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfind_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 22:01:43 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 17:25:44 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	*pathfind(t_list **path, t_point from, t_point to, struct s_map *map)
{
	t_list	*queue;
	t_point	*pt;
	t_point	nears[4];
	int		i;

	ft_lstclear(path, free);
	if (!pathfind_init(map, to, &queue, &pt))
		return (NULL);
	while (queue)
	{
		pt = ft_lstpop(&queue);
		i = pathfind_nears_get(map, *pt, nears);
		while (--i >= 0)
		{
			if (pathfind_nears_check(&queue, map, *pt, nears[i]) == false)
				return (pathfind_deinit(&queue, pt));
			if (nears[i].x == from.x && nears[i].y == from.y)
			{
				pathfind_deinit(&queue, pt);
				return (pathfind_construct(path, from, map));
			}
		}
		free(pt);
	}
	return (NULL);
}

bool	pathfind_init(struct s_map *map, t_point target, t_list **queue,
					  t_point **pt)
{
	uint32_t	y;
	uint32_t	x;

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

bool	pathfind_nears_check(t_list **queue, struct s_map *map, t_point pt_from,
							 t_point pt_near)
{
	t_list	*near_lst;

	if (map->grid_bfs[pt_near.y][pt_near.x] == 0)
	{
		map->grid_bfs[pt_near.y][pt_near.x] = \
				map->grid_bfs[pt_from.y][pt_from.x] + 1;
		near_lst = ft_lstnew(malloc(sizeof(pt_near)));
		if (near_lst == NULL || near_lst->content == NULL)
			return (false);
		*(t_point *)near_lst->content = pt_near;
		ft_lstadd_back(queue, near_lst);
	}
	return (true);
}

int	pathfind_nears_get(struct s_map *map, t_point pt, t_point *nears)
{
	const t_point	dirs[] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
	t_point			check;
	int				i;
	int				counter;

	i = 0;
	counter = 0;
	while (i < 4)
	{
		check = (t_point){pt.x + dirs[i].x, pt.y + dirs[i].y};
		if ((uint32_t)check.x < map->size.x && \
				(uint32_t)check.y < map->size.y && \
				map->grid_bfs[check.y][check.x] == 0)
			nears[counter++] = check;
		i++;
	}
	return (counter);
}

void	*pathfind_deinit(t_list **queue, t_point *pt)
{
	ft_lstclear(queue, free);
	free(pt);
	return (NULL);
}

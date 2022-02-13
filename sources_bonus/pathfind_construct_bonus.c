/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfind_construct_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 17:28:38 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 17:41:46 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	*pathfind_construct(t_list **path, t_point from, struct s_map *map)
{
	const t_point	dirs[] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
	const int		rnd = random() % 4;
	t_point			check;
	t_list			*lst;
	int				i;

	i = -1;
	while (++i < 4)
	{
		check.x = from.x + dirs[(i + rnd) % 4].x;
		check.y = from.y + dirs[(i + rnd) % 4].y;
		if (map->grid_bfs[check.y][check.x] &&
			map->grid_bfs[check.y][check.x] < map->grid_bfs[from.y][from.x])
		{
			lst = ft_lstnew(malloc(sizeof(check)));
			if (lst == NULL || lst->content == NULL)
				return (pathfind_deinit(path, lst->content));
			*(t_point *)lst->content = check;
			ft_lstadd_front(path, lst);
			if (map->grid_bfs[check.y][check.x] == 1)
				return (ft_lstreverse(path));
			return (pathfind_construct(path, check, map));
		}
	}
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 15:20:37 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/19 18:06:30 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

t_door	*door_find(t_game *game, t_point cell)
{
	t_list	*cur_list;
	t_door	*door;

	cur_list = game->doors;
	while (cur_list)
	{
		door = (t_door *)cur_list->content;
		if (door->cell.x == cell.x && door->cell.y == cell.y)
			return (door);
		cur_list = cur_list->next;
	}
	return (NULL);
}

void	doors(t_game *g)
{
	t_list		*cur_list;
	t_door		*door;

	cur_list = g->doors;
	while (cur_list)
	{
		door = (t_door *)cur_list->content;
		if (door->opening == true)
		{
			if (door->part_opened < 1.0f)
				door->part_opened = fminf(1.0f, door->part_opened + 1.0f / \
							(DOOR_ANIM_TICKS + DOOR_ANIM_TICKS * door->secret));
			if (door->secret)
				door_secret_open(g, door);
			if (door->ticks_to_close > 0)
				--door->ticks_to_close;
			if (door->ticks_to_close == 0 && door->locked_count == 0)
				door_autoclose(g, door);
		}
		else if (door->part_opened > 0.0f)
		{
			if ((int)g->p.pos.x == door->cell.x && (int)g->p.pos.y == door->cell.y)
				door_open(g, door, true);
			else
				door->part_opened = fmaxf(0.0f, \
                                door->part_opened - 1.0f / DOOR_ANIM_TICKS);
		}
		cur_list = cur_list->next;
	}
}

void	door_autoclose(t_game *game, t_door *door)
{
	if (((int)game->p.pos.x == door->cell.x && \
		 (int)game->p.pos.y == door->cell.y) == false)
	{
		door->opening = false;
		door_sound(game, door);
	}
}

void	door_secret_open(t_game *game, t_door *door)
{
	if (door->part_opened == 1.0f && \
		(door->secret_target.x != door->cell.x || \
		 door->secret_target.y != door->cell.y))
	{
		game->map.grid[door->cell.y][door->cell.x] = '.';
		if (door->secret_target.x > door->cell.x)
			door->cell.x++;
		else if (door->secret_target.x < door->cell.x)
			door->cell.x--;
		else if (door->secret_target.y > door->cell.y)
			door->cell.y++;
		else if (door->secret_target.y < door->cell.y)
			door->cell.y--;
		game->map.grid[door->cell.y][door->cell.x] = '=';
		if (door->cell.x != door->secret_target.x || \
			door->cell.y != door->secret_target.y)
			door->part_opened = 0.0f;
		else
		{
			game->map.grid[door->cell.y][door->cell.x] = \
					door->secret_texture_id + '0';
			bfs_grid_update(game);
		}
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 15:20:37 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/26 15:11:50 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	doors(t_game *game)
{
	t_list		*cur_list;
	t_door		*door;

	cur_list = game->doors;
	while (cur_list)
	{
		door = (t_door *)cur_list->content;
		if (door->opening == true)
		{
			if (door->part_opened < 1.0f)
				door->part_opened = fminf(1.0f,
										  door->part_opened + 1.0f / ANIM_DOOR_TICKS);
			if (door->ticks_to_close > 0)
				--door->ticks_to_close;
			if (door->ticks_to_close == 0 &&
				((int)game->p.pos.x == door->cell.x &&
				 (int)game->p.pos.y == door->cell.y) == false)
			{
				door->opening = false;
				sound_play(game, &game->audio.sound[SND_DOOR_CLOSE],
						   (t_fpoint){door->cell.x + 0.5, door->cell.y + 0.5});
			}
		}
		else if (door->opening == false && door->part_opened > 0)
			door->part_opened = fmaxf(0.0f,
									  door->part_opened - 1.0f / ANIM_DOOR_TICKS);
		cur_list = cur_list->next;
	}
}

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

void	door_open(t_game *g, t_point cell, bool by_player)
{
	t_door	*door;

	if (ft_memchr(CHAR_DOORS, g->map.grid[cell.y][cell.x], sizeof(CHAR_DOORS)))
	{
		door = door_find(g, cell);
		if (by_player)
			door->opening ^= true;
		else
			door->opening = true;
		door_sound(g, door);
	}
	else if (by_player)
		sound_play(g, &g->audio.sound[SND_PLAYER_NOACTION], T_FPT_NULL);
}

void	door_sound(t_game *game, t_door *door)
{
	if (door->sound)
		cs_stop_sound(door->sound);
	if (door->opening == true)
	{
		door->sound = sound_play(game, &game->audio.sound[SND_DOOR_OPEN],
						(t_fpoint){door->cell.x + 0.5f, door->cell.y + 0.5f});
		if (door->sound)
			door->sound->sample_index = (float)ANIM_DOOR_TICKS / TICKS_PER_SEC *
					door->part_opened * door->sound->loaded_sound->sample_rate;
		door->ticks_to_close = DOOR_TIMER_TICKS;
	}
	else
	{
		door->sound = sound_play(game, &game->audio.sound[SND_DOOR_CLOSE],
						(t_fpoint){door->cell.x + 0.5f, door->cell.y + 0.5f});
		door->sound->sample_index = (float)ANIM_DOOR_TICKS / TICKS_PER_SEC *
			(1.0f - door->part_opened) * door->sound->loaded_sound->sample_rate;
		door->ticks_to_close = 0;
	}
}
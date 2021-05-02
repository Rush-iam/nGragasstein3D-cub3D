/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 15:20:37 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/02 20:44:58 by ngragas          ###   ########.fr       */
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
				door->part_opened = fminf(1.0f, door->part_opened + 1.0f /
					(DOOR_ANIM_TICKS + DOOR_ANIM_TICKS * door->secret));
			if (door->secret && door->part_opened == 1.0f &&
				(door->secret_target.x != door->cell.x ||
				door->secret_target.y != door->cell.y))
			{
				game->map.grid[door->cell.y][door->cell.x] = '.';
				door->cell.x += (door->secret_target.x > door->cell.x) - (door->secret_target.x < door->cell.x);
				door->cell.y += (door->secret_target.y > door->cell.y) - (door->secret_target.y < door->cell.y);
				game->map.grid[door->cell.y][door->cell.x] = '=';
				if (door->cell.x != door->secret_target.x ||
					door->cell.y != door->secret_target.y)
					door->part_opened = 0.0f;
				else
				{
					game->map.grid[door->cell.y][door->cell.x] = door->secret_texture_id + '0';
					bfs_grid_update(game);
				}
			}
			if (door->ticks_to_close > 0)
				--door->ticks_to_close;
			if (door->ticks_to_close == 0 &&
				((int)door->opener_pos->x == door->cell.x &&
				(int)door->opener_pos->y == door->cell.y) == false &&
				((int)game->p.pos.x == door->cell.x &&
				(int)game->p.pos.y == door->cell.y) == false)
			{
				door->opening = false;
				door_sound(game, door);
			}
		}
		else if (door->part_opened > 0.0f)
			door->part_opened = fmaxf(0.0f,
								door->part_opened - 1.0f / DOOR_ANIM_TICKS);
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

void	door_open(t_game *g, t_point cell, t_fpoint *opener_pos, bool by_player)
{
	t_door	*door;

	if (ft_memchr(CHAR_DOORS, g->map.grid[cell.y][cell.x], sizeof(CHAR_DOORS))
		|| g->map.grid[cell.y][cell.x] == *CHAR_DOOR_SECRET)
	{
		door = door_find(g, cell);
		if ((door->opener_pos && (int)door->opener_pos->x == door->cell.x &&
			(int)door->opener_pos->y == door->cell.y) ||
			(door->secret && door->opening) ||
			(by_player == false && door->opening == true))
			return ;
		if (door->secret)
			door->secret_target = (t_point){
					door->cell.x + 2 * (g->column[g->center.x].dir == 'E') -
					2 * (g->column[g->center.x].dir == 'W'),
					door->cell.y + 2 * (g->column[g->center.x].dir == 'S') -
					2 * (g->column[g->center.x].dir == 'N')};
		else if (door->end_level)
		{
			string_add(g, END_LEVEL_MESSAGE, 20, COLOR_CYAN_F);
			g->map.grid[cell.y][cell.x] = *CHAR_ELEVATOR_ON;
			music_play(g, &g->audio.music[MUSIC_END_ID]);
		}
		door->opener_pos = opener_pos;
		if (by_player)
			door->opening ^= true;
		else if (door->opening == false)
			door->opening = true;
		if (door->secret == true || door->end_level == true)
			door->ticks_to_close = -1;
		else
			door->ticks_to_close = DOOR_TIMER_TICKS * (door->opening == true);
		door_sound(g, door);
	}
	else if (by_player)
		sound_play(g, &g->audio.sound[SND_PLAYER_NOACTION], (t_fpoint){0, 0});
}

void	door_sound(t_game *game, t_door *door)
{
	t_snd	*sound;

	if (door->secret)
		sound = &game->audio.sound[SND_DOOR_SECRET];
	else if (door->end_level)
		sound = &game->audio.sound[SND_ELEVATOR];
	else if (door->opening)
		sound = &game->audio.sound[SND_DOOR_OPEN];
	else
		sound = &game->audio.sound[SND_DOOR_CLOSE];
	if (door->sound)
		cs_stop_sound(door->sound);
	door->sound = sound_play(game, sound,
						(t_fpoint){door->cell.x + 0.5f, door->cell.y + 0.5f});
	if (door->sound == NULL)
		return ;
	if (door->opening)
		door->sound->sample_index = (float)DOOR_ANIM_TICKS / TICKS_PER_SEC *
					door->part_opened * door->sound->loaded_sound->sample_rate;
	else
		door->sound->sample_index = (float)DOOR_ANIM_TICKS / TICKS_PER_SEC *
			(1.0f - door->part_opened) * door->sound->loaded_sound->sample_rate;
}
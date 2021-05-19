/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors_extra_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 14:37:44 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/19 20:52:41 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	door_open_try(t_game *g, t_point cell, bool by_player)
{
	t_door	*door;

	if (ft_memchr(CHAR_DOORS, g->map.grid[cell.y][cell.x], sizeof(CHAR_DOORS)) \
		== NULL && g->map.grid[cell.y][cell.x] != *CHAR_SECRET)
	{
		if (by_player)
			sound_play(g, &g->audio.sound[SND_PLAYER_NOACTION], \
						(t_fpoint){0, 0});
		return ;
	}
	door = door_find(g, cell);
	if (by_player == false)
		door->locked_count++;
	if (door->locked_count > 1 - (by_player == true) || \
		(door->secret && door->opening))
		return ;
	door_open(g, door, by_player);
}

void	door_open(t_game *g, t_door *door, bool by_player)
{
	if (door->secret)
		door->secret_target = (t_point){\
				door->cell.x + 2 * (g->column[g->center.x].dir == 'E') - \
				2 * (g->column[g->center.x].dir == 'W'), \
				door->cell.y + 2 * (g->column[g->center.x].dir == 'S') - \
				2 * (g->column[g->center.x].dir == 'N')};
	else if (door->end_level)
	{
		message_add(g, END_LEVEL_MESSAGE, 20, COLOR_CYAN_F);
		g->map.grid[door->cell.y][door->cell.x] = *CHAR_ELEVATOR_ON;
		music_play(g, &g->audio.music[MUSIC_END_ID]);
	}
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

void	door_locker(t_game *g, t_point cell, bool lock)
{
	t_door	*door;

	door = door_find(g, cell);
	if (!door)
		return ;
	if (lock)
		door->locked_count++;
	else
		door->locked_count--;
}

void	door_sound(t_game *game, t_door *d)
{
	t_snd	*sound;

	if (d->secret)
		sound = &game->audio.sound[SND_DOOR_SECRET];
	else if (d->end_level)
		sound = &game->audio.sound[SND_ELEVATOR];
	else if (d->opening)
		sound = &game->audio.sound[SND_DOOR_OPEN];
	else
		sound = &game->audio.sound[SND_DOOR_CLOSE];
	if (d->sound && d->sound->snd \
				 && d->sound->sourcepos.x == d->cell.x + 0.5f \
				 && d->sound->sourcepos.y == d->cell.y + 0.5f)
		cs_stop_sound(d->sound->snd);
	d->sound = sound_play(game, sound, \
						(t_fpoint){d->cell.x + 0.5f, d->cell.y + 0.5f});
	if (d->sound == NULL)
		return ;
	if (d->opening)
		d->sound->snd->sample_index = (float)DOOR_ANIM_TICKS / TICKS_PER_SEC * \
				d->part_opened * d->sound->snd->loaded_sound->sample_rate;
	else
		d->sound->snd->sample_index = (float)DOOR_ANIM_TICKS / TICKS_PER_SEC * \
		(1.0f - d->part_opened) * d->sound->snd->loaded_sound->sample_rate;
}

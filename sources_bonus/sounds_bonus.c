/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sounds_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 17:30:14 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/25 21:46:06 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	sounds(t_game *game)
{
	unsigned	i;

	i = 0;
	while (i < sizeof(game->audio.playing) / sizeof(*game->audio.playing))
	{
		if (game->audio.playing[i].snd != NULL)
		{
			if (game->audio.playing[i].snd->active == false)
				game->audio.playing[i].snd = NULL;
			else if (game->audio.playing[i].sourcepos.x != 0.0)
				sound_adjust_pan(&game->p, game->audio.playing[i]);
		}
		i++;
	}
}

void	sound_play(t_game *game, t_snd *sound, t_fpoint sourcepos)
{
	unsigned		i;
	cs_context_t	*target_ctx;

	if (sound == NULL || sound->file.channels[0] == NULL)
		return ;
	i = 0;
	while (game->audio.playing[i].snd != NULL &&
				i < sizeof(game->audio.playing) / sizeof(*game->audio.playing))
		i++;
	if (i == sizeof(game->audio.playing) / sizeof(*game->audio.playing))
		return ;
	if (sound->file.sample_rate == 7042)
		target_ctx = game->audio.ctx7;
	else if (sound->file.sample_rate == 22050)
		target_ctx = game->audio.ctx22;
	else
		target_ctx = game->audio.ctx;
	game->audio.playing[i].snd = cs_play_sound(target_ctx, sound->props);
	game->audio.playing[i].sourcepos = sourcepos;
	if (sourcepos.x != 0.0f)
		sound_adjust_pan(&game->p, game->audio.playing[i]);
}

void	sound_adjust_pan(struct s_player *pl, struct s_playing_sound sound)
{
	float	distance;
	float	angle;

	distance = fabsf(sound.sourcepos.x - pl->pos.x) +
			   fabsf(sound.sourcepos.y - pl->pos.y);
	angle = atan2f(sound.sourcepos.y - pl->pos.y,
					sound.sourcepos.x - pl->pos.x) - pl->angle;
	if (angle < -M_PI_F)
		angle += PI2_F;
	cs_set_pan(sound.snd, 0.5f + sinf(angle) / 2.0f);
	sound.snd->volume0 = fmaxf(1.0f - 0.05f * distance, 0.5f);
	sound.snd->volume1 = sound.snd->volume0;
}
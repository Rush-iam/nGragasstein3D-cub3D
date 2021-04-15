/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pickups_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 17:52:01 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/15 22:53:01 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

bool	pickup(t_game *game, enum e_objtype type)
{
	if (type == T_ENEMY || type == T_DECOR)
		return (false);
	if (type == T_AMMO && game->p.ammo >= 99)
		return (false);
	if (game->p.health >= 100 && (type == T_HEALTH_M || type == T_HEALTH_L))
		return (false);
	if ((game->p.ammo == 0 && (type == T_AMMO || type == T_AMMO_ENEMY)) ||
		type == T_RIFLE)
	{
		pickup_get(game, type);
		if (game->p.weapons_mask & W_RIFLE_MASK)
			player_set_weapon(game, W_RIFLE);
		else
			player_set_weapon(game, W_PISTOL);
	}
	else
		pickup_get(game, type);
	game->p.ammo = ft_min(game->p.ammo, 99);
	if ((type == T_HEALTH_L || type == T_HEALTH_M) && game->p.health > 100)
		game->p.health = 100;
	printf("STATUS: Health = %hd; Ammo = %hd; Score = %hd\n",
		   game->p.health, game->p.ammo, game->p.score);
	game->effect = (struct s_effect){15, 30, EF_FLASH, COLOR_YELLOW, .2};
	pickup_sound(game, type);
	return (true);
}

void	pickup_drop(t_game *game, t_fpoint pos, enum e_objtype type, t_img *img)
{
	t_object	*obj;

	if ((obj = ft_calloc(1, sizeof(t_object))) == NULL)
		terminate(game, ERR_MEM, "Memory allocation failed (object)");
	obj->pos = pos;
	if (img == NULL)
		obj->sprite = &game->sprite[type - 1 + (sizeof(CHAR_DECOR) - 1)];
	else
		obj->sprite = img;
	obj->type = type;
	object_add(game, &game->objects, obj);
}

void	pickup_get(t_game *game, enum e_objtype item)
{
	if (item == T_HEALTH_L)
		game->p.health += VAL_HEALTH_L;
	else if (item == T_HEALTH_M)
		game->p.health += VAL_HEALTH_M;
	else if (item == T_HEALTH_XL)
	{
		if (game->endleveltext)
			string_add(game, ft_strdup(game->endleveltext), 60, COLOR_GREEN);
		game->p.health = VAL_HEALTH_XL;
	}
	else if (item == T_AMMO)
		game->p.ammo += VAL_AMMO_M;
	else if (item == T_AMMO_ENEMY)
		game->p.ammo += VAL_AMMO_S;
	else if (item == T_RIFLE)
	{
		game->p.ammo += VAL_AMMO_M;
		game->p.weapons_mask |= W_RIFLE_MASK;
	}
	else if (item == T_BONUS_XL)
		game->p.score += VAL_SCORE_XL;
	else if (item == T_BONUS_L)
		game->p.score += VAL_SCORE_L;
	else if (item == T_BONUS_M)
		game->p.score += VAL_SCORE_M;
	else if (item == T_BONUS_S)
		game->p.score += VAL_SCORE_S;
}

void	pickup_sound(t_game *game, enum e_objtype item)
{
	if (item == T_AMMO || item == T_AMMO_ENEMY)
		sound_play(game, &game->audio.sound[SND_PICKUP_AMMO], T_FPT_NULL);
	else if (item == T_RIFLE)
		sound_play(game, &game->audio.sound[SND_PICKUP_RIFLE], T_FPT_NULL);
	else if (item == T_HEALTH_M)
		sound_play(game, &game->audio.sound[SND_PICKUP_HEALTH_M], T_FPT_NULL);
	else if (item == T_HEALTH_L)
		sound_play(game, &game->audio.sound[SND_PICKUP_HEALTH_L], T_FPT_NULL);
	else if (item == T_HEALTH_XL)
		sound_play(game, &game->audio.sound[SND_PICKUP_HEALTH_XL], T_FPT_NULL);
	else if (item == T_BONUS_S)
		sound_play(game, &game->audio.sound[SND_PICKUP_BONUS_S], T_FPT_NULL);
	else if (item == T_BONUS_M)
		sound_play(game, &game->audio.sound[SND_PICKUP_BONUS_M], T_FPT_NULL);
	else if (item == T_BONUS_L)
		sound_play(game, &game->audio.sound[SND_PICKUP_BONUS_L], T_FPT_NULL);
	else if (item == T_BONUS_XL)
		sound_play(game, &game->audio.sound[SND_PICKUP_BONUS_XL], T_FPT_NULL);
}

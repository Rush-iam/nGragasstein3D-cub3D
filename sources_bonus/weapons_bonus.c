/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapons_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 16:12:36 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/25 20:57:10 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	weapon(t_game *game, struct s_weapon *weapon)
{
	if (weapon->tick > 0)
	{
		if (++weapon->tick >= weapon->ticks)
		{
			weapon->tick = 0;
			weapon->frame = 0;
			if (game->p.ammo == 0 && game->p.weapon_cur != W_KNIFE)
				player_set_weapon(game, W_KNIFE);
		}
		weapon->frame = weapon->frames * weapon->tick / weapon->ticks;
		if (game->p.weapon_shot == false && weapon->frame == SHOT_FRAME_ID)
		{
			weapon_shoot(game, game->p.target);
			game->p.weapon_shot = true;
		}
		else if (weapon->frame == 3)
			game->p.weapon_shot = false;
		if (game->p.weapon_cur == W_RIFLE && weapon->frame == 3 && game->p.ammo)
			weapon->lock = false;
	}
}

void	player_set_weapon(t_game *game, enum e_weapon weapon)
{
	if (game->p.weapons_mask == 0 || (weapon != W_KNIFE && game->p.ammo == 0) ||
		(weapon == W_PISTOL && (game->p.weapons_mask & W_PISTOL_MASK) == 0) ||
		(weapon == W_RIFLE && (game->p.weapons_mask & W_RIFLE_MASK) == 0))
		return ;
	game->p.weapon_cur = weapon;
	if (game->p.weapon_cur == W_KNIFE)
	{
		ft_memcpy(game->p.weapon.animation, ANIM_KNIFE, sizeof(ANIM_KNIFE));
		game->p.weapon.frames = sizeof(ANIM_KNIFE);
	}
	else if (game->p.weapon_cur == W_PISTOL)
	{
		ft_memcpy(game->p.weapon.animation, ANIM_PISTOL, sizeof(ANIM_PISTOL));
		game->p.weapon.frames = sizeof(ANIM_PISTOL);
	}
	else if (game->p.weapon_cur == W_RIFLE)
	{
		ft_memcpy(game->p.weapon.animation, ANIM_RIFLE, sizeof(ANIM_RIFLE));
		game->p.weapon.frames = sizeof(ANIM_RIFLE);
	}
	game->p.weapon.ticks = game->p.weapon.frames * ANIM_TICKS;
	game->p.weapon_pos = (t_upoint){
	game->win_center.x - game->p.weapon_img[game->p.weapon_cur][0].size.x / 2,
	game->img.size.y - game->p.weapon_img[game->p.weapon_cur][0].size.y};
}

void	draw_weapon(t_game *game, struct s_weapon *weapon)
{
	mlx_put_image_to_window(game->mlx, game->win, game->p.weapon_img
					[game->p.weapon_cur][weapon->animation[weapon->frame]].ptr,
					game->p.weapon_pos.x, game->p.weapon_pos.y);
}

void	weapon_sound(t_game *game, enum e_weapon weapon)
{
	if (weapon == W_KNIFE)
		sound_play(game, &game->audio.sound[SND_KNIFE], T_FPT_NULL);
	else if (weapon == W_PISTOL)
		sound_play(game, &game->audio.sound[SND_PISTOL], T_FPT_NULL);
	else if (weapon == W_RIFLE)
		sound_play(game, &game->audio.sound[SND_RIFLE], T_FPT_NULL);
}

void	weapon_shoot(t_game *g, t_object *target)
{
	unsigned	damage;

	weapon_sound(g, g->p.weapon_cur);
	if (g->p.weapon_cur != W_KNIFE)
		g->p.ammo--;
	if (target)
	{
		if (g->p.weapon_cur == W_KNIFE && target->distance_real < 1)
			damage = DMG_KNIFE_MIN + arc4random() %
					(1 + DMG_KNIFE_MAX - DMG_KNIFE_MIN);
		else if (g->p.weapon_cur != W_KNIFE)
			damage = DMG_SHOT_MIN + arc4random() %
					(1 + DMG_SHOT_MAX - DMG_SHOT_MIN);
		else
			return ;
		target->e->health -= damage;
		if (target->e->health <= 0)
		{
			pickup_drop(g, target->pos, T_AMMO_ENEMY,
						&g->sprite[sizeof(CHAR_DECOR) - 1 + T_AMMO - 1]);
			enemy_set_state(g, target, &g->enemyset[ENEMY_GUARD], ST_DEATH);
			g->p.score += VAL_SCORE_KILL;
		}
		else
			enemy_set_state(g, target, &g->enemyset[ENEMY_GUARD], ST_PAIN);
		printf("bam! ammo left: %hd RND damage: %u\n", g->p.ammo, damage);
	}
}

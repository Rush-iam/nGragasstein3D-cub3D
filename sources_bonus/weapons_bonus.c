/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapons_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 16:12:36 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/02 20:48:33 by ngragas          ###   ########.fr       */
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
			if (game->p.weapon_cur != W_KNIFE)
				game->p.weapon_noise = true;
		}
		else if (weapon->frame == 3)
			game->p.weapon_shot = false;
		if (game->p.weapon_cur == W_RIFLE && weapon->frame == 3 && game->p.ammo)
			weapon->lock = false;
	}
}

void	player_set_weapon(t_game *g, enum e_weapon weapon)
{
	if (g->p.weapons_mask == 0 || (weapon != W_KNIFE && g->p.ammo == 0) ||
		(weapon == W_PISTOL && (g->p.weapons_mask & W_PISTOL_MASK) == 0) ||
		(weapon == W_RIFLE && (g->p.weapons_mask & W_RIFLE_MASK) == 0))
		return ;
	g->p.weapon_cur = weapon;
	if (g->p.weapon_cur == W_KNIFE)
	{
		ft_memcpy(g->p.weapon.animation, ANIM_KNIFE, sizeof(ANIM_KNIFE) - 1);
		g->p.weapon.frames = sizeof(ANIM_KNIFE) - 1;
	}
	else if (g->p.weapon_cur == W_PISTOL)
	{
		ft_memcpy(g->p.weapon.animation, ANIM_PISTOL, sizeof(ANIM_PISTOL) - 1);
		g->p.weapon.frames = sizeof(ANIM_PISTOL) - 1;
	}
	else if (g->p.weapon_cur == W_RIFLE)
	{
		ft_memcpy(g->p.weapon.animation, ANIM_RIFLE, sizeof(ANIM_RIFLE) - 1);
		g->p.weapon.frames = sizeof(ANIM_RIFLE) - 1;
	}
	g->p.weapon.ticks = g->p.weapon.frames * ANIM_TICKS;
	g->p.weapon_pos = (t_upoint){
			g->center.x - g->p.weapon_img[g->p.weapon_cur][0].size.x / 2,
			g->img.size.y - g->p.weapon_img[g->p.weapon_cur][0].size.y};
	g->hud.needs_redraw = true;
}

void	draw_weapon(t_game *g, struct s_weapon *weapon)
{
	const int	offset = g->p.velocity * (ANIM_WEAPON_MAX_OFFSET *\
						sinf((float)((int)g->tick % (TICKS_PER_SEC * 2 / 3) -\
						TICKS_PER_SEC / 3) / (TICKS_PER_SEC / 3) * M_PI_F));

	mlx_put_image_to_window(g->mlx, g->win,
		g->p.weapon_img[g->p.weapon_cur][weapon->animation[weapon->frame]].ptr,
		g->p.weapon_pos.x - g->key.mdir.x / 4 + offset,
		g->p.weapon_pos.y + ft_max(0, -g->key.mdir.y / 3) + abs(offset));
}

void	weapon_sound(t_game *game, enum e_weapon weapon)
{
	if (weapon == W_KNIFE)
		sound_play(game, &game->audio.sound[SND_KNIFE], (t_fpoint){0, 0});
	else if (weapon == W_PISTOL)
		sound_play(game, &game->audio.sound[SND_PISTOL], (t_fpoint){0, 0});
	else if (weapon == W_RIFLE)
		sound_play(game, &game->audio.sound[SND_RIFLE], (t_fpoint){0, 0});
}

void	weapon_shoot(t_game *g, t_object *target)
{
	unsigned	damage;

	weapon_sound(g, g->p.weapon_cur);
	if (g->p.weapon_cur != W_KNIFE)
		g->p.ammo--;
	if (target && (g->p.weapon_cur != W_KNIFE || target->distance_real < 1.0f))
	{
		if (g->p.weapon_cur == W_KNIFE)
			damage = DMG_KNIFE_MIN + arc4random() %
					(1 + DMG_KNIFE_MAX - DMG_KNIFE_MIN);
		else
			damage = DMG_SHOT_MIN + arc4random() %
					(1 + DMG_SHOT_MAX - DMG_SHOT_MIN);
		target->e->health -= damage;
		if (target->e->health <= 0)
		{
			pickup_drop(g, target->pos, T_AMMO_ENEMY,
						&g->sprite[sizeof(CHAR_DECOR) - 1 + T_AMMO - 1]);
			enemy_set_state(g, target, ST_DEATH);
			g->p.score += VAL_SCORE_KILL;
		}
		else
			enemy_set_state(g, target, ST_PAIN);
	}
	g->hud.needs_redraw = true;
}

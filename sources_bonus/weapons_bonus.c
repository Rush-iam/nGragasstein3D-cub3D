/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapons_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 16:12:36 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/17 21:21:46 by ngragas          ###   ########.fr       */
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

void	weapon_shoot(t_game *game, t_object *target)
{
	unsigned	damage;

	if (game->p.weapon_cur != W_KNIFE)
		game->p.ammo--;
	if (target)
	{
		if (game->p.weapon_cur == W_KNIFE && target->distance_real < 1)
			damage = DMG_KNIFE_MIN + arc4random() % (DMG_KNIFE - DMG_KNIFE_MIN);
		else if (game->p.weapon_cur != W_KNIFE)
			damage = DMG_SHOT_MIN + arc4random() % (1 + DMG_SHOT - DMG_SHOT_MIN);
		else
			return ;
		target->e->health -= damage;
		if (target->e->health <= 0)
		{
			object_drop(game, target->pos, T_AMMO_ENEMY,
				&game->sprite[sizeof(CHAR_DECOR) - 1 + T_AMMO - 1]);
			enemy_set_state(target, &game->imgset[ENEMY_ID_GUARD], S_DEAD);
			game->p.score += VAL_SCORE_KILL;
		}
		else
			enemy_set_state(target, &game->imgset[ENEMY_ID_GUARD], S_PAIN);
		printf("bam! ammo left: %hd RND damage: %u\n", game->p.ammo, damage);
	}
}

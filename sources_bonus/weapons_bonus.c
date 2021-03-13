/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapons_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 16:12:36 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/13 16:01:57 by ngragas          ###   ########.fr       */
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
			weapon_shoot(game);
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
	if (weapon == W_KNIFE)
	{
		ft_memcpy(game->p.weapon.animation, ANIM_KNIFE, sizeof(ANIM_KNIFE));
		game->p.weapon.frames = sizeof(ANIM_KNIFE);
	}
	else if (weapon == W_PISTOL)
	{
		ft_memcpy(game->p.weapon.animation, ANIM_PISTOL, sizeof(ANIM_PISTOL));
		game->p.weapon.frames = sizeof(ANIM_PISTOL);
	}
	else if (weapon == W_RIFLE)
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
	mlx_put_image_to_window(game->mlx, game->win,game->p.weapon_img
					[game->p.weapon_cur][weapon->animation[weapon->frame]].ptr,
					game->p.weapon_pos.x, game->p.weapon_pos.y);
}

void	object_drop(t_game *game, t_fpoint pos, enum e_objtype type)
{
	t_object	*obj;

	if ((obj = ft_calloc(1, sizeof(t_object))) == NULL)
		terminate(game, ERR_MEM, "Memory allocation failed (object)");
	obj->pos = pos;
	obj->sprite = &game->sprite[type - 1 + (sizeof(CHAR_DECOR) - 1)];
	obj->type = type;
	object_add(game, &game->objects, obj);
}

void	weapon_shoot(t_game *game)
{
	if (game->p.weapon_cur != W_KNIFE)
		game->p.ammo--;
	if (game->p.target)
	{
		if (game->p.weapon_cur == W_KNIFE && game->p.target->distance_real < 1)
			game->p.target->e->health -= 13;
		else if (game->p.weapon_cur != W_KNIFE)
			game->p.target->e->health -= 13;
		if (game->p.target->e->health <= 0)
		{
			object_drop(game, game->p.target->pos, T_AMMO);
			game->p.target->e->state = S_DEAD;
			game->p.target->sprite = &game->sprite[16];
			game->p.score += VAL_SCORE_KILL;
		}
	}
	printf("bam! ammo left: %hd\n", game->p.ammo);
}

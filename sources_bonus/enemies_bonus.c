/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 17:53:39 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/13 22:40:28 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	enemy(t_game *game, t_object *obj)
{
	if (obj->e->state != ST_DEATH)
	{
		if (obj->distance_real < game->column[game->win_center.x].distance &&
			obj->render.start.x <= (int)game->win_center.x &&
			obj->render.end.x >= (int)game->win_center.x &&	(!game->p.target ||
							obj->distance_real < game->p.target->distance_real))
			game->p.target = obj;
	}
	obj->e->p_to_angle = obj->atan_diff - (obj->e->angle - M_PI_F);
	if (obj->e->p_to_angle < -M_PI_F)
		obj->e->p_to_angle += PI2_F;
	else if (obj->e->p_to_angle >= M_PI_F)
		obj->e->p_to_angle -= PI2_F;
	enemy_logic(game, obj);
	enemy_sprite(game, obj);
}

void	enemy_sprite(t_game *game, t_object *obj)
{
	if (obj->e->state == ST_WAIT)
		obj->e->frame = (8 - (int)ceil(obj->e->p_to_angle / M_PI_4_F - 0.5f)) % 8;
	else if (obj->e->state == ST_WALK)
	{
		obj->e->frame = obj->e->frames * obj->e->tick / obj->e->ticks %
														ANIM_ENEMY_WALK_FRAMES;
		obj->e->imgset = game->enemyset[ENEMY_GUARD].
				walk[(8 - (int)ceil(obj->e->p_to_angle / M_PI_4_F - 0.5f)) % 8];
	}
	else
		obj->e->frame = obj->e->frames * obj->e->tick / obj->e->ticks;
	obj->sprite = &obj->e->imgset[obj->e->frame];
}

void	enemy_logic(t_game *game, t_object *obj)
{
	bool	see;

	if (obj->e->state == ST_DEATH && obj->e->tick + 1 == obj->e->ticks)
		return ;
	see = false;
	if (obj->e->state != ST_DEATH && fabsf(obj->e->p_to_angle) < ENEMY_FOV_HALF)
		see = ray_intersect_distance(game, obj->atan_diff) > obj->distance_real;
	if (obj->e->state == ST_WAIT && see == false)
		return ;
	if (see == true)
	{
		if (obj->e->alarmed == false)
			enemy_sound(game, obj, SND_ENEMY_ALARM);
		obj->e->angle = obj->atan_diff + M_PI;
		obj->e->alarmed = true;
		obj->e->target = game->p.pos;
	}
	obj->e->target_distance = distance(obj->pos, obj->e->target);
	if (++obj->e->tick >= obj->e->ticks ||
		(obj->e->state == ST_ATTACK && see == false))
	{
		if (see == true && (obj->e->state == ST_WAIT ||
						obj->e->state == ST_PAIN || obj->e->state == ST_WALK))
			enemy_set_state(game, obj, ST_ATTACK);
		else if (see == true && obj->e->state == ST_ATTACK &&
											obj->e->target_distance < 2.0f)
			enemy_set_state(game, obj, ST_ATTACK);
		else if (see == true && obj->e->state == ST_ATTACK)
			enemy_set_state(game, obj,
			(enum e_objstate[]){ST_ATTACK, ST_WALK, ST_WALK}[arc4random() % 3]);
		else if (see == false && obj->e->state == ST_ATTACK)
			enemy_set_state(game, obj, ST_WALK);
		else if (see == false && obj->e->state == ST_WALK &&
									obj->e->target_distance > 0.1f)
			enemy_set_state(game, obj, ST_WALK);
		else
			enemy_set_state(game, obj, ST_WAIT);
	}
	if (obj->e->state == ST_ATTACK && obj->e->shot == false &&
			obj->e->frame == SHOT_FRAME_ID)
	{
		enemy_sound(game, obj, SND_ENEMY_ATTACK);
		enemy_shoot(game, obj);
		obj->e->shot = true;
	}
}

void	enemy_shoot(t_game *g, t_object *obj)
{
	unsigned	damage;
	float		power;
	unsigned	miss_chance;

	power = fminf(1.0f, 1.0f / sqrtf(obj->distance_real));
	miss_chance = ft_umin(ENEMY_MISS_MAX, (int)sqrtf(obj->distance_real) * 10);
	if (arc4random() % 100 < miss_chance)
		return ;
	damage = ENEMY_DMG_MIN +
			 arc4random() % (1 + ENEMY_DMG_MAX - ENEMY_DMG_MIN) * power;
	g->p.health -= damage;
	if (damage >= (ENEMY_DMG_MIN + ENEMY_DMG_MAX) / 2)
		sound_play(g, &g->audio.sound[SND_PLAYER_PAIN], T_FPT_NULL);
	printf("Enemy shot you! -%u HP. Health: %hd\n", damage, g->p.health);
	g->effect = (struct s_effect){15, 30, EF_FLASH, COLOR_RED, damage / 100.0f};
}

void	enemy_set_state(t_game *g, t_object *obj, enum e_objstate state)
{
	obj->e->state = state;
	if (state == ST_WAIT)
	{
		obj->e->imgset = g->enemyset[obj->e->type].wait;
		obj->e->frames = 3;
	}
	else if (state == ST_WALK)
	{
		obj->e->imgset = g->enemyset[obj->e->type].walk[0];
		obj->e->frames = 4 + arc4random() % (int)obj->distance_real & ~1;
		if (obj->e->path == NULL ||
		obj->e->path[obj->e->path_len - 1].x != (unsigned)obj->e->target.x ||
		obj->e->path[obj->e->path_len - 1].y != (unsigned)obj->e->target.y)
			obj->e->path = pathfind(g, obj);
	}
	else if (state == ST_ATTACK)
	{
		obj->e->imgset = g->enemyset[obj->e->type].attack;
		obj->e->frames = 3;
		obj->e->ticks *= ENEMY_SHOT_DELAY;
		obj->e->shot = false;
	}
	else if (state == ST_PAIN)
	{
		obj->e->imgset = &g->enemyset[obj->e->type].pain[obj->angle_to_p < 0];
		obj->e->frames = 1;
		obj->e->angle = obj->atan_diff + M_PI_F;
	}
	else if (state == ST_DEATH)
	{
		enemy_sound(g, obj, SND_ENEMY_DEATH);
		obj->e->imgset = g->enemyset[obj->e->type].death;
		obj->e->frames = 5;
	}
	obj->e->tick = 0;
	obj->e->ticks = obj->e->frames * ANIM_ENEMY_TICKS;
	if (state == ST_ATTACK)
		obj->e->ticks *= ENEMY_SHOT_DELAY;
	obj->e->frame = 0;
	obj->sprite = &obj->e->imgset[obj->e->frame];
}

void	enemy_sound(t_game *game, t_object *obj, enum e_sound sound_type)
{
	if (sound_type == SND_ENEMY_ALARM)
		sound_play(game, &game->enemyset[obj->e->type].s_alarm, obj->pos);
	else if (sound_type == SND_ENEMY_ATTACK)
		sound_play(game, &game->enemyset[obj->e->type].s_attack, obj->pos);
	else if (sound_type == SND_ENEMY_DEATH)
		sound_play(game, &game->enemyset[obj->e->type].s_death[arc4random() %
			ft_umax(1, game->enemyset[obj->e->type].s_death_count)], obj->pos);
}

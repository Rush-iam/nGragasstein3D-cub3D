/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 17:53:39 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 21:04:13 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	enemy(t_game *game, t_object *obj)
{
	if (obj->e->state != ST_DEATH)
		if (obj->distance_real < game->column[game->center.x].distance && \
			obj->render.start.x <= game->center.x && \
			obj->render.end.x >= game->center.x && (!game->p.target || \
							obj->distance_real < game->p.target->distance_real))
			game->p.target = obj;
	obj->e->p_to_angle = obj->atan_diff - (obj->e->angle - M_PI_F);
	if (obj->e->p_to_angle < -M_PI_F)
		obj->e->p_to_angle += PI2_F;
	else if (obj->e->p_to_angle >= M_PI_F)
		obj->e->p_to_angle -= PI2_F;
	enemy_logic(game, obj);
	if (obj->e->state == ST_WALK)
	{
		if (!obj->e->path || \
			(obj->e->path_target.x != (int)obj->e->target.x || \
			obj->e->path_target.y != (int)obj->e->target.y))
		{
			pathfind(&obj->e->path, (t_point){obj->pos.x, obj->pos.y}, \
					(t_point){obj->e->target.x, obj->e->target.y}, &game->map);
			obj->e->path_target = (t_point){obj->e->target.x, obj->e->target.y};
		}
		enemy_move(game, obj, *(t_point *)obj->e->path->content);
	}
	enemy_sprite(game, obj);
}

void	enemy_move(t_game *game, t_object *o, t_point move_int)
{
	if (o->e->path->next)
		o->e->walk_angle = atan2f(move_int.y + 0.5f - o->pos.y, \
									move_int.x + 0.5f - o->pos.x);
	else
		o->e->walk_angle = atan2f(o->e->target.y - o->pos.y, \
									o->e->target.x - o->pos.x);
	if (o->e->target.x == o->e->location.x && \
		o->e->target.y == o->e->location.y)
		o->e->angle = o->e->walk_angle;
	o->pos.x += cosf(o->e->walk_angle) * ENEMY_SPEED * (1 + o->e->alarmed);
	o->pos.y += sinf(o->e->walk_angle) * ENEMY_SPEED * (1 + o->e->alarmed);
	if (game->map.grid[move_int.y][move_int.x] != '.')
		door_open_try(game, move_int, &o->pos, false);
	if ((int)o->pos.x == move_int.x && (int)o->pos.y == move_int.y)
	{
		if (o->e->path->next || o->e->see)
			free(ft_lstpop(&o->e->path));
		else if (fabsf(o->e->target.x - o->pos.x) < 0.1f && \
				  fabsf(o->e->target.y - o->pos.y) < 0.1f)
		{
			o->e->tick = o->e->ticks;
			free(ft_lstpop(&o->e->path));
		}
	}
}

void	enemy_sprite(t_game *game, t_object *obj)
{
	float	angle;

	if (obj->e->state == ST_WAIT)
		obj->e->frame = \
				(8 - (int)ceilf(obj->e->p_to_angle / M_PI_4_F - 0.5f)) % 8;
	else if (obj->e->state == ST_WALK)
	{
		angle = obj->atan_diff - (obj->e->walk_angle - M_PI_F);
		if (angle < -M_PI_F)
			angle += PI2_F;
		else if (angle >= M_PI_F)
			angle -= PI2_F;
		obj->e->frame = obj->e->frames * obj->e->tick / obj->e->ticks % \
														ANIM_ENEMY_WALK_FRAMES;
		obj->e->imgset = game->enemyset[obj->e->type].\
						walk[(8 - (int)ceilf(angle / M_PI_4_F - 0.5f)) % 8];
	}
	else
		obj->e->frame = obj->e->frames * obj->e->tick / obj->e->ticks;
	obj->sprite = &obj->e->imgset[obj->e->frame];
}

void	enemy_sound(t_game *game, t_object *obj, enum e_sound sound_type)
{
	if (sound_type == SND_ENEMY_ALARM)
		sound_play(game, &game->enemyset[obj->e->type].s_alarm, obj->pos);
	else if (sound_type == SND_ENEMY_ATTACK)
		sound_play(game, &game->enemyset[obj->e->type].s_attack, obj->pos);
	else if (sound_type == SND_ENEMY_DEATH)
		sound_play(game, &game->enemyset[obj->e->type].s_death[arc4random() % \
			ft_umax(1, game->enemyset[obj->e->type].s_death_count)], obj->pos);
}

void	enemy_shoot(t_game *g, t_object *obj)
{
	uint32_t	damage;
	float		power;
	uint32_t	miss_chance;

	enemy_sound(g, obj, SND_ENEMY_ATTACK);
	obj->e->shot = true;
	power = fminf(1.0f, 1.0f / sqrtf(obj->distance_real));
	miss_chance = ft_umin(ENEMY_MISS_MAX, (int)sqrtf(obj->distance_real) * 10);
	if (arc4random() % 100 < miss_chance)
		return ;
	damage = ENEMY_DMG_MIN + \
			 arc4random() % (1 + ENEMY_DMG_MAX - ENEMY_DMG_MIN) * power;
	g->p.health -= damage;
	g->p.health = ft_max(0, g->p.health);
	if (damage >= (ENEMY_DMG_MIN + ENEMY_DMG_MAX) / 2)
		sound_play(g, &g->audio.sound[SND_PLAYER_PAIN], (t_fpoint){0, 0});
	g->effect = (struct s_effect){15, 30, EF_FLASH, COLOR_RED, damage / 100.0f};
	g->hud.needs_redraw = true;
}

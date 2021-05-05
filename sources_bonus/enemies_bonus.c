/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 17:53:39 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/05 23:57:55 by ngragas          ###   ########.fr       */
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
		if (!obj->e->path || (obj->e->path_target.x != (int)obj->e->target.x \
			|| obj->e->path_target.y != (int)obj->e->target.y))
		{
			pathfind(&obj->e->path, (t_point){obj->pos.x, obj->pos.y}, \
					(t_point){obj->e->target.x, obj->e->target.y}, &game->map);
			obj->e->path_target = (t_point){obj->e->target.x, obj->e->target.y};
		}
		if (obj->e->path && obj->e->type != ENEMY_DOG)
			enemy_move(game, obj, *(t_point *)obj->e->path->content);
		else if (obj->e->path)
			enemy_chaotic_move(game, obj, *(t_point *)obj->e->path->content);
	}
	enemy_sprite(game, obj);
}

void	enemy_move(t_game *game, t_object *ob, t_point move_int)
{
	if (ob->e->path->next)
		ob->e->walk_angle = atan2f(move_int.y + 0.5f - ob->pos.y, \
									move_int.x + 0.5f - ob->pos.x);
	else
		ob->e->walk_angle = atan2f(ob->e->target.y - ob->pos.y, \
									ob->e->target.x - ob->pos.x);
	if (ob->e->target.x == ob->e->location.x && \
		ob->e->target.y == ob->e->location.y)
		ob->e->angle = ob->e->walk_angle;
	ob->pos.x += cosf(ob->e->walk_angle) * ob->e->speed * (1 + ob->e->alarmed);
	ob->pos.y += sinf(ob->e->walk_angle) * ob->e->speed * (1 + ob->e->alarmed);
	if (game->map.grid[move_int.y][move_int.x] != '.')
		door_open_try(game, move_int, &ob->pos, false);
	if ((int)ob->pos.x == move_int.x && (int)ob->pos.y == move_int.y)
	{
		if (ob->e->path->next || ob->e->see)
			free(ft_lstpop(&ob->e->path));
		else if (fabsf(ob->e->target.x - ob->pos.x) < 0.1f && \
				  fabsf(ob->e->target.y - ob->pos.y) < 0.1f)
		{
			ob->e->tick = ob->e->ticks;
			free(ft_lstpop(&ob->e->path));
		}
	}
}

void	enemy_chaotic_move(t_game *game, t_object *ob, t_point move_int)
{
	if (ob->distance_real < 1.0f)
	{
		ft_lstclear(&ob->e->path, free);
		ob->e->tick = ob->e->ticks;
		return ;
	}
	if (game->map.grid[move_int.y][move_int.x] != '.' && \
		door_find(game, move_int)->part_opened < 0.8f)
	{
		ft_lstclear(&ob->e->path, free);
		ob->e->angle += M_PI_F;
		return ;
	}
	if (ob->e->path->next)
		ob->e->walk_angle = atan2f(move_int.y + 0.5f - ob->pos.y, \
									move_int.x + 0.5f - ob->pos.x);
	else
		ob->e->walk_angle = atan2f(ob->e->target.y - ob->pos.y, \
									ob->e->target.x - ob->pos.x);
	if (ob->e->see == false)
		ob->e->angle = ob->e->walk_angle;
	ob->pos.x += cosf(ob->e->walk_angle) * ob->e->speed * (1 + ob->e->alarmed);
	ob->pos.y += sinf(ob->e->walk_angle) * ob->e->speed * (1 + ob->e->alarmed);
	if ((int)ob->pos.x == move_int.x && (int)ob->pos.y == move_int.y)
	{
		if (ob->e->path->next || ob->e->see)
			free(ft_lstpop(&ob->e->path));
		else if (fabsf(ob->e->target.x - ob->pos.x) < 0.1f && \
				  fabsf(ob->e->target.y - ob->pos.y) < 0.1f)
		{
			ob->e->tick = ob->e->ticks;
			free(ft_lstpop(&ob->e->path));
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

void	enemy_range_attack(t_game *g, t_object *obj)
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
	damage = ENEMY_GUARD_DMG_MIN + arc4random() % \
						(1 + ENEMY_GUARD_DMG_MAX - ENEMY_GUARD_DMG_MIN) * power;
	g->p.health -= damage;
	g->p.health = ft_max(0, g->p.health);
	if (damage >= (ENEMY_GUARD_DMG_MIN + ENEMY_GUARD_DMG_MAX) / 2)
		sound_play(g, &g->audio.sound[SND_PLAYER_PAIN], (t_fpoint){0, 0});
	g->effect = (struct s_effect){15, 30, EF_FLASH, COLOR_RED, damage / 100.0f};
	g->hud.needs_redraw = true;
}

void	enemy_melee_attack(t_game *g, t_object *obj)
{
	uint32_t	damage;

	enemy_sound(g, obj, SND_ENEMY_ATTACK);
	obj->e->shot = true;
	if (obj->distance_real > 1.0f)
		return ;
	damage = ENEMY_DOG_DMG_MIN + \
			 arc4random() % (1 + ENEMY_DOG_DMG_MAX - ENEMY_DOG_DMG_MIN);
	g->p.health -= damage;
	g->p.health = ft_max(0, g->p.health);
	if (damage >= (ENEMY_DOG_DMG_MIN + ENEMY_DOG_DMG_MAX) / 2)
		sound_play(g, &g->audio.sound[SND_PLAYER_PAIN], (t_fpoint){0, 0});
	g->effect = (struct s_effect){15, 30, EF_FLASH, COLOR_RED, damage / 100.0f};
	g->hud.needs_redraw = true;
}

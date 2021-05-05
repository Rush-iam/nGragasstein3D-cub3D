/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_logic_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 19:49:34 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/05 23:32:56 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	enemy_logic(t_game *game, t_object *obj)
{
	if (obj->e->state == ST_DEATH && obj->e->tick + 1 == obj->e->ticks)
		return ;
	enemy_look(game, obj);
	if (obj->e->state == ST_WAIT && obj->e->alarmed == false)
		return ;
	obj->e->tick++;
	if (obj->e->tick >= obj->e->ticks || \
		(obj->e->state == ST_ATTACK && obj->e->see == false))
	{
		if (obj->e->type == ENEMY_GUARD)
			enemy_range_think(game, obj);
		else if (obj->e->type == ENEMY_DOG)
			enemy_melee_think(game, obj);
	}
	if (obj->e->state == ST_ATTACK && obj->e->shot == false && \
			obj->e->frame == SHOT_FRAME_ID)
	{
		if (obj->e->type == ENEMY_GUARD)
			enemy_range_attack(game, obj);
		else if (obj->e->type == ENEMY_DOG)
			enemy_melee_attack(game, obj);
	}
}

void	enemy_look(t_game *game, t_object *obj)
{
	obj->e->see = false;
	if (obj->e->state != ST_DEATH && \
		(game->p.weapon_noise || fabsf(obj->e->p_to_angle) < ENEMY_FOV_HALF))
		obj->e->see = \
			obj->distance_real < ray_intersect_distance(game, obj->atan_diff);
	if (obj->e->see)
	{
		if (obj->e->alarmed == false)
		{
			enemy_sound(game, obj, SND_ENEMY_ALARM);
			if (obj->e->state == ST_WALK)
				obj->e->tick = obj->e->ticks;
		}
		obj->e->angle = obj->atan_diff + M_PI;
		obj->e->alarmed = true;
		obj->e->target = game->p.pos;
	}
}

void	enemy_range_think(t_game *game, t_object *obj)
{
	if (obj->e->see == true)
	{
		if (obj->e->state != ST_ATTACK || obj->distance_real < 2.0f)
			enemy_range_set_state(game, obj, ST_ATTACK);
		else
			enemy_range_set_state(game, obj, \
            (enum e_state[]){ST_ATTACK, ST_WALK, ST_WALK}[arc4random() % 3]);
	}
	else if (obj->e->see == false)
	{
		if (obj->e->state == ST_ATTACK || obj->e->state == ST_WAIT || \
			(obj->e->state == ST_WALK && obj->e->path))
			enemy_range_set_state(game, obj, ST_WALK);
		else if ((int)obj->pos.x != (int)obj->e->location.x || \
				 (int)obj->pos.y != (int)obj->e->location.y)
		{
			obj->e->target = obj->e->location;
			enemy_range_set_state(game, obj, ST_WALK);
			obj->e->alarmed = false;
		}
		else
			enemy_range_set_state(game, obj, ST_WAIT);
	}
}

void	enemy_melee_think(t_game *game, t_object *obj)
{
	const t_point	dirs[] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}, \
							{-1, -1}, {1, 1}, {-1, 1}, {1, -1}};
	t_point			check;
	int				i;
	int				rnd = arc4random() % 8;

	if (obj->e->alarmed == false || obj->e->path == NULL)
	{
		i = 0;
		while (i < 8)
		{
			check = (t_point){obj->pos.x + dirs[(i + rnd) % 8].x, \
								obj->pos.y + dirs[(i + rnd) % 8].y};
			if ((uint32_t)check.x < game->map.size.x && \
			(uint32_t)check.y < game->map.size.y && \
			game->map.grid[check.y][check.x] == '.')
			{
				obj->e->target = (t_fpoint){check.x + 0.5f, check.y + 0.5f};
				break ;
			}
			i++;
		}
	}
	if (obj->e->see == true && obj->distance_real < 1.0f)
		enemy_melee_set_state(game, obj, ST_ATTACK);
	else
		enemy_melee_set_state(game, obj, ST_WALK);
}

float	ray_intersect_distance(t_game *game, float angle)
{
	t_ray		x1;
	t_ray		y1;
	t_fpoint	distance;
	const float	tan_cur_angle = tanf(angle);

	if (angle < -M_PI_2_F || angle >= M_PI_2_F)
		x1 = ray_intersect_x(game, -1, -tan_cur_angle);
	else
		x1 = ray_intersect_x(game, 1, tan_cur_angle);
	if (angle >= 0.0f)
		y1 = ray_intersect_y(game, 1 / tan_cur_angle, 1);
	else
		y1 = ray_intersect_y(game, -1 / tan_cur_angle, -1);
	distance.x = hypotf(x1.pos.x - game->p.pos.x, x1.pos.y - game->p.pos.y);
	distance.y = hypotf(y1.pos.x - game->p.pos.x, y1.pos.y - game->p.pos.y);
	return (fminf(distance.x, distance.y));
}

void	enemy_range_set_state(t_game *g, t_object *obj, enum e_state state)
{
	const short	frames[] = {3, 4, 3, 1, 5};
	const t_img	*imgsets[] = {g->enemyset[obj->e->type].wait, \
		g->enemyset[obj->e->type].walk[0], g->enemyset[obj->e->type].attack, \
						&g->enemyset[obj->e->type].pain[obj->angle_to_p < 0], \
						g->enemyset[obj->e->type].death};

	obj->e->state = state;
	obj->e->frames = frames[state];
	obj->e->imgset = (t_img *)imgsets[state];
	if (state == ST_WAIT)
		obj->e->angle = obj->e->location_angle;
	else if (state == ST_WALK)
		obj->e->frames += arc4random() % ((int)obj->distance_real + 1) & ~1;
	else if (state == ST_ATTACK)
		obj->e->shot = false;
	else if (state == ST_PAIN)
		obj->e->angle = obj->atan_diff + M_PI_F;
	else if (state == ST_DEATH)
		pickup_drop(g, obj->pos, T_AMMO_ENEMY, \
                        &g->sprite[sizeof(CHAR_DECOR) - 1 + T_AMMO - 1]);
	obj->e->tick = 0;
	obj->e->frame = 0;
	obj->e->ticks = obj->e->frames * ANIM_ENEMY_TICKS;
	if (state == ST_ATTACK || (state == ST_WALK && obj->e->alarmed == false))
		obj->e->ticks *= ANIM_ENEMY_SLOWMO;
}

void	enemy_melee_set_state(t_game *g, t_object *obj, enum e_state state)
{
	const short	frames[] = {0, 4, 6, 0, 4};
	const t_img	*imgsets[] = {NULL, \
							g->enemyset[obj->e->type].walk[0], \
							g->enemyset[obj->e->type].attack, \
							NULL, \
							g->enemyset[obj->e->type].death};

	obj->e->state = state;
	obj->e->frames = frames[state];
	obj->e->imgset = (t_img *)imgsets[state];
	if (state == ST_ATTACK)
		obj->e->shot = false;
	else if (state == ST_DEATH)
		enemy_sound(g, obj, SND_ENEMY_DEATH);
	else if (state == ST_WALK && obj->e->see == false)
	{
	}
	obj->e->tick = 0;
	obj->e->frame = 0;
	obj->e->ticks = obj->e->frames * ANIM_ENEMY_TICKS;
	if (obj->e->alarmed && state != ST_DEATH)
		obj->e->ticks = obj->e->ticks * 2 / 3;
}

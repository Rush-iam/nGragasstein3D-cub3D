/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:33:03 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/23 22:35:09 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	objects(t_game *g)
{
	t_list		*cur_list;
	t_list		*next_list;
	t_object	*obj;
	t_fpoint	diff;

	next_list = g->objects;
	g->p.target = NULL;
	while ((cur_list = next_list))
	{
		next_list = cur_list->next;
		obj = (t_object *)cur_list->content;
		diff = (t_fpoint){obj->pos.x - g->p.pos.x, obj->pos.y - g->p.pos.y};
		obj->distance = g->p.vector.x * diff.x + g->p.vector.y * diff.y;
		obj->distance_real = hypot(diff.x, diff.y);
		obj->atan_diff = atan2(diff.y, diff.x);
		draw_object_properties(g, obj);
		if (obj->type == T_ENEMY)
			enemy_settings(g, obj);
		if (obj->distance_real < 0.5 && object_pickup(g, obj->type))
			ft_lstremove(&g->objects, cur_list);
	}
}

void	enemy_settings(t_game *game, t_object *obj)
{
	if (obj->e->state != ST_DEAD)
	{
		if (obj->distance_real < game->column[game->win_center.x].distance &&
			obj->render.start.x <= (int)game->win_center.x &&
			obj->render.end.x >= (int)game->win_center.x &&	(!game->p.target ||
			obj->distance_real < game->p.target->distance_real))
			game->p.target = obj;
	}
	obj->e->p_to_angle = obj->atan_diff - (obj->e->angle - M_PI);
	if (obj->e->p_to_angle < -M_PI)
		obj->e->p_to_angle += PI2;
	else if (obj->e->p_to_angle > M_PI)
		obj->e->p_to_angle -= PI2;
	if (obj->e->state == ST_WAIT)
		obj->e->imgset = &game->imgset[ENEMY_ID_GUARD].
				wait[(8 - (int)ceil(obj->e->p_to_angle / M_PI_4 - 0.5)) % 8];
	enemy_logic(game, obj);
	obj->sprite = &obj->e->imgset[obj->e->frame];
}

void	enemy_shoot(t_game *game, t_object *obj)
{
	unsigned	damage;
	float		power;
	unsigned	miss_chance;

	power = fminf(1, 1 / sqrt(obj->distance_real));
	miss_chance = ft_umin(ENEMY_MISS_MAX, sqrt(obj->distance_real) * 10);
	if (arc4random() % 100 < miss_chance)
		return ;
	damage = ENEMY_DMG_MIN +
				arc4random() % (1 + ENEMY_DMG_MAX - ENEMY_DMG_MIN) * power;
	game->p.health -= damage;
	printf("Enemy shot you! -%u HP. Health: %hd\n", damage, game->p.health);
	game->effect = (struct s_effect){30, 15, EF_FLASH, COLOR_RED, damage / 100.};
}

void	enemy_logic(t_game *game, t_object *obj)
{
	bool	see;

	if (obj->e->state == ST_DEAD && obj->e->tick + 1 >= obj->e->ticks)
		return ;
	see = false;
	if (obj->e->state != ST_DEAD && fabsf(obj->e->p_to_angle) < ENEMY_FOV_HALF)
		see = ray_intersect_distance(game, obj->atan_diff) > obj->distance_real;
	if (obj->e->state == ST_WAIT && see == false)
		return ;
	if (see == true)
	{
		obj->e->angle = obj->atan_diff + M_PI;
		obj->e->alarmed = true;
	}
	if (++obj->e->tick >= obj->e->ticks ||
								(obj->e->state == ST_ATTACK && see == false))
	{
		if (see == true && (obj->e->state == ST_WAIT || obj->e->state == ST_PAIN))
			enemy_set_state(obj, &game->imgset[ENEMY_ID_GUARD], ST_ATTACK);
		else
			enemy_set_state(obj, &game->imgset[ENEMY_ID_GUARD], ST_WAIT);
	}
	if (obj->e->state != ST_WAIT)
		obj->e->frame = obj->e->frames * obj->e->tick / obj->e->ticks;
	if (obj->e->state == ST_ATTACK && obj->e->shot == false &&
											obj->e->frame == SHOT_FRAME_ID)
	{
		enemy_shoot(game, obj);
		obj->e->shot = true;
	}
}

void	enemy_set_state(t_object *obj, t_imgset *imgset, enum e_objstate state)
{
	obj->e->state = state;
	if (state == ST_WAIT)
	{
		obj->e->imgset = imgset->wait;
		obj->e->frames = 2;
	}
//	else if (state == ST_WALK)
//	{
//		obj->e->imgset = imgset->walk[0];
//		obj->e->frames = 4;
//	}
	else if (state == ST_ATTACK)
	{
		obj->e->imgset = imgset->attack;
		obj->e->frames = 3;
		obj->e->ticks *= ENEMY_SHOT_DELAY;
		obj->e->shot = false;
	}
	else if (state == ST_PAIN)
	{
		obj->e->imgset = &imgset->pain[obj->angle_to_p < 0];
		obj->e->frames = 1;
		obj->e->alarmed = true;
		obj->e->angle = obj->atan_diff + M_PI;
	}
	else if (state == ST_DEAD)
	{
		obj->e->imgset = imgset->dead;
		obj->e->frames = 5;
	}
	obj->e->tick = 0;
	obj->e->ticks = obj->e->frames * ANIM_ENEMY_TICKS;
	if (state == ST_ATTACK)
		obj->e->ticks *= ENEMY_SHOT_DELAY;
	obj->e->frame = 0;
	obj->sprite = &obj->e->imgset[obj->e->frame];
}

void	object_drop(t_game *game, t_fpoint pos, enum e_objtype type, t_img *img)
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

void	object_add(t_game *game, t_list **dst, void *obj)
{
	t_list	*new;

	if ((new = ft_lstnew(obj)) == NULL)
		terminate(game, ERR_MEM, "Memory allocation failed (obj_list)");
	ft_lstadd_front(dst, new);
}

int		objects_sort(t_object *obj1, t_object *obj2)
{
	return (obj1->distance < obj2->distance);
}

bool	object_pickup(t_game *game, enum e_objtype type)
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
		object_pickup_add(game, type);
		if (game->p.weapons_mask & W_RIFLE_MASK)
			player_set_weapon(game, W_RIFLE);
		else
			player_set_weapon(game, W_PISTOL);
	}
	else
		object_pickup_add(game, type);
	game->p.ammo = ft_min(game->p.ammo, 99);
	if ((type == T_HEALTH_L || type == T_HEALTH_M) && game->p.health > 100)
		game->p.health = 100;
	printf("PICK UP! Health = %hd; Ammo = %hd; Score = %hd\n",
					game->p.health, game->p.ammo, game->p.score);
	game->effect = (struct s_effect){30, 15, EF_FLASH, COLOR_YELLOW, .2};
	return (true);
}

void	object_pickup_add(t_game *game, enum e_objtype type)
{
	if (type == T_HEALTH_L)
		game->p.health += VAL_HEALTH_L;
	else if (type == T_HEALTH_M)
		game->p.health += VAL_HEALTH_M;
	else if (type == T_HEALTH_XL)
		game->p.health = VAL_HEALTH_XL;
	else if (type == T_AMMO)
		game->p.ammo += VAL_AMMO_M;
	else if (type == T_AMMO_ENEMY)
		game->p.ammo += VAL_AMMO_S;
	else if (type == T_RIFLE)
	{
		game->p.ammo += VAL_AMMO_M;
		game->p.weapons_mask |= W_RIFLE_MASK;
	}
	else if (type == T_BONUS_XL)
		game->p.score += VAL_SCORE_XL;
	else if (type == T_BONUS_L)
		game->p.score += VAL_SCORE_L;
	else if (type == T_BONUS_M)
		game->p.score += VAL_SCORE_M;
	else if (type == T_BONUS_S)
		game->p.score += VAL_SCORE_S;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:33:03 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/16 23:14:02 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	enemy(t_game *game, t_object *enemy)
{
	(void)game;
	if (enemy->e->state == S_WAIT)
		return ;
	if (enemy->e->state == S_DEAD && enemy->e->tick + 1 >= enemy->e->ticks)
		return ;
	if (++enemy->e->tick >= enemy->e->ticks)
	{
		enemy_set_state(enemy, &game->imgset[ENEMY_ID_GUARD], S_WAIT);
		return ;
	}
	enemy->e->frame = enemy->e->frames * enemy->e->tick / enemy->e->ticks;
	if (enemy->e->state == S_ATTACK)
	{
		if (enemy->e->shot == false && enemy->e->frame == SHOT_FRAME_ID)
		{
//			enemy_shoot(game);
			enemy->e->shot = true;
		}
		else if (enemy->e->frame == 3)
			enemy->e->shot = false;
	}
}

void	enemy_set_state(t_object *obj, t_imgset *imgset, enum e_objstate state)
{
	obj->e->state = state;
	if (state == S_WAIT)
		obj->e->imgset = imgset->wait;
	else if (state == S_WALK)
	{
		obj->e->imgset = imgset->walk[0];
		obj->e->frames = 4;
	}
	else if (state == S_ATTACK)
	{
		obj->e->imgset = imgset->attack;
		obj->e->frames = 3;
	}
	else if (state == S_PAIN)
	{
		obj->e->imgset = &imgset->pain[obj->e->angle_from_p < 0];
		obj->e->frames = 1;
	}
	else if (state == S_DEAD)
	{
		obj->e->imgset = imgset->dead;
		obj->e->frames = 5;
	}
	obj->e->tick = 0;
	obj->e->ticks = obj->e->frames * ANIM_ENEMY_TICKS;
	obj->e->frame = 0;
}

void	objects(t_game *g)
{
	t_list		*cur_list;
	t_list		*next_list;
	t_object	*obj;
	t_fpoint	diff;

	cur_list = g->objects;
	g->p.target = NULL;
	while (cur_list)
	{
		next_list = cur_list->next;
		obj = (t_object *)cur_list->content;
		diff = (t_fpoint){obj->pos.x - g->p.pos.x, obj->pos.y - g->p.pos.y};
		obj->distance_real = hypot(diff.x, diff.y);
		if (obj->type != T_ENEMY && obj->type != T_DECOR &&
					obj->distance_real < 0.5 && object_pickup(g, obj->type))
			ft_lstremove(&g->objects, cur_list);
		if (obj->type == T_ENEMY && obj->e->targeted && obj->e->state != S_DEAD)
			g->p.target = obj;
		if (obj->type == T_ENEMY)
			enemy(g, obj);
		cur_list = next_list;
	}
}

void	draw_enemy_settings(t_game *game, t_object *obj, float angle, float atan2_diff)
{
	obj->e->targeted = false;
	obj->e->angle_from_p = angle;
	obj->e->angle_to_p = atan2_diff;
	if (fabs(obj->e->angle - obj->e->angle_to_p - PI2) <= M_PI + M_PI_4 / 2)
		obj->e->angle_to_p += PI2;
	obj->e->angle_to_p -= obj->e->angle - M_PI + M_PI_4 / 2;
	if (obj->e->state == S_WAIT)
		obj->e->imgset = &game->imgset[ENEMY_ID_GUARD].wait[
										7 - (int)(obj->e->angle_to_p / M_PI_4)];
	obj->sprite = &obj->e->imgset[obj->e->frame];
}

void	draw_objects(t_game *g, t_list *cur_list)
{
	t_object	*obj;
	float		atan2_diff;
	float		angle;
	t_fpoint	diff;

	while (cur_list)
	{
		obj = (t_object *)cur_list->content;
		diff = (t_fpoint){obj->pos.x - g->p.pos.x, obj->pos.y - g->p.pos.y};
		obj->distance = g->p.vector.x * diff.x + g->p.vector.y * diff.y;
		if (obj->distance > 0.1)
		{
			atan2_diff = atan2(diff.y, diff.x);
			if (fabs(g->p.angle - atan2_diff - PI2) <= M_PI)
				angle = atan2_diff + PI2 - g->p.angle;
			else
				angle = atan2_diff - g->p.angle;
			if (obj->type == T_ENEMY)
				draw_enemy_settings(g, obj, angle, atan2_diff);
			if (fabs(angle) < g->fov + M_PI_4)
				draw_sprite(g, obj, angle);
		}
		cur_list = cur_list->next;
	}
}

void	object_add(t_game *game, t_list **dst, t_object *obj)
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
	if (type == T_AMMO && game->p.ammo >= 99)
		return (false);
	if (game->p.health >= 100 && (type == T_HEALTH_M || type == T_HEALTH_L))
		return (false);
	if (game->p.ammo == 0 && (type == T_AMMO || type == T_AMMO_ENEMY))
		player_set_weapon(game,
					(game->p.weapons_mask & W_RIFLE_MASK) ? W_RIFLE : W_PISTOL);
	else if (type == T_RIFLE)
		player_set_weapon(game, W_RIFLE);
	object_pickup_add(game, type);
	if (game->p.ammo > 99)
		game->p.ammo = 99;
	else if (game->p.health > 100 && (type == T_HEALTH_L || type == T_HEALTH_M))
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

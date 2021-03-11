/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:33:03 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/11 17:41:56 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	objects(t_game *g)
{
	t_list		*cur_list;
	t_object	*obj;
	t_fpoint	diff;

	cur_list = g->objects;
	while (cur_list)
	{
		obj = (t_object *)cur_list->content;
		diff = (t_fpoint){obj->pos.x - g->p.pos.x, obj->pos.y - g->p.pos.y};
		obj->distance_real = hypot(diff.x, diff.y);
		if (obj->distance_real < 0.5)
			cur_list = object_pickup(g, cur_list, obj->type);
		else
			cur_list = cur_list->next;
	}
}

void	draw_objects(t_game *g)
{
	t_list		*cur_list;
	t_object	*obj;
	double		angle;
	t_fpoint	diff;

	ft_lstsort(&g->objects, objects_sort);
	cur_list = g->objects;
	while (cur_list)
	{
		obj = (t_object *)cur_list->content;
		diff = (t_fpoint){obj->pos.x - g->p.pos.x, obj->pos.y - g->p.pos.y};
		obj->distance = g->p.vector.x * diff.x + g->p.vector.y * diff.y;
		if (obj->distance > 0.1)
		{
			angle = atan2(diff.y, diff.x);
			if (fabs(g->p.angle - angle - PI2) <= M_PI)
				angle += PI2;
			angle -= g->p.angle;
			if (fabs(angle) < g->fov + M_PI_4)
				draw_sprite(g, obj, angle);
		}
		cur_list = cur_list->next;
	}
}

int		objects_sort(t_object *obj1, t_object *obj2)
{
	return (obj1->distance < obj2->distance);
}

t_list	*object_pickup(t_game *game, t_list *cur_list, enum e_objtype type)
{
	const t_list	*next = cur_list->next;

	if (type == T_DECOR || type == T_ENEMY)
		return ((t_list *)next);
	if (type == T_AMMO && game->p.ammo >= 99)
		return ((t_list *)next);
	if ((type == T_HEALTH_M || type == T_HEALTH_L) && game->p.health >= 100)
		return ((t_list *)next);
	object_pickup_add(game, type);
	if (game->p.ammo > 99)
		game->p.ammo = 99;
	else if (game->p.health > 100 && (type == T_HEALTH_L || type == T_HEALTH_M))
		game->p.health = 100;
	printf("UP type: %u. Health = %hd; Ammo = %hd; Score = %hd\n", type,
		   game->p.health, game->p.ammo, game->p.score);
	game->effect = (struct s_effect){30, 15, EF_FLASH, COLOR_YELLOW, .2};
	ft_lstremove(&game->objects, cur_list);
	return ((t_list *)next);
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
		game->p.weapons |= WEAPON_RIFLE;
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:33:03 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/08 23:34:29 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	objects(t_game *game)
{
	t_list		*cur_list;
	t_object	*obj;
	double		angle;
	t_fpoint	diff;

	ft_lstsort(&game->objects, objects_sort);
	cur_list = game->objects;
	while (cur_list)
	{
		obj = (t_object *)cur_list->content;
		diff = (t_fpoint){obj->pos.x - game->p.pos.x, obj->pos.y - game->p.pos.y};
		if ((obj->distance = game->p.vector.x * diff.x + game->p.vector.y * diff.y) > 0.1)
		{
			angle = atan2(diff.y, diff.x);
			if (fabs(game->p.angle - angle - PI2) <= M_PI)
				angle += PI2;
			angle -= game->p.angle;
			if (fabs(angle) < game->fov + M_PI_4)
				draw_sprite(game, obj, angle);
		}
		if ((obj->distance_real = hypot(diff.x, diff.y)) < 0.5)
			cur_list = object_pickup(game, cur_list, obj->type);
		else
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

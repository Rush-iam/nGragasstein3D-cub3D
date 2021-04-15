/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:33:03 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/15 16:22:11 by ngragas          ###   ########.fr       */
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
		obj->distance = g->p.vect.x * diff.x + g->p.vect.y * diff.y;
		obj->distance_real = hypotf(diff.x, diff.y);
		obj->atan_diff = atan2f(diff.y, diff.x);
		draw_object_properties(g, obj);
		if (obj->type == T_ENEMY)
			enemy(g, obj);
		if (obj->distance_real < 0.5f && pickup(g, obj->type))
			ft_lstremove(&g->objects, cur_list);
	}
	g->p.weapon_noise = false;
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

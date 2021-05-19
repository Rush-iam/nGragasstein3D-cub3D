/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_resources_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 16:21:09 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/19 16:23:03 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	free_resources(t_game *g)
{
	uint32_t	i;

	ft_lstclear(&g->doors, free);
	ft_lstclear(&g->objects, free_object);
	if (g->texture_ceil.ptr)
		mlx_destroy_image(g->mlx, g->texture_ceil.ptr);
	if (g->texture_floor.ptr)
		mlx_destroy_image(g->mlx, g->texture_floor.ptr);
	if (g->img_bg.ptr)
		mlx_destroy_image(g->mlx, g->img_bg.ptr);
	if (g->map.grid)
		ft_free_ptr_array((void **)g->map.grid, g->map.size.y);
	if (g->map.grid_bfs)
		ft_free_ptr_array((void **)g->map.grid_bfs, g->map.size.y);
	free_image_array(g, g->texture, sizeof(g->texture) / sizeof(*g->texture));
	free_image_array(g, g->sprite, sizeof(g->sprite) / sizeof(*g->sprite));
	i = -1U;
	while (++i < sizeof(g->p.weapon_img) / sizeof(*g->p.weapon_img))
		free_image_array(g, g->p.weapon_img[i], \
				sizeof(g->p.weapon_img[i]) / sizeof(*g->p.weapon_img[i]));
	i = 0;
	while (i < sizeof(g->audio.music) / sizeof(*g->audio.music))
		cs_free_sound(&g->audio.music[i++].file);
	i = 0;
	while (i < sizeof(g->audio.sound) / sizeof(*g->audio.sound))
		cs_free_sound(&g->audio.sound[i++].file);
}

void	free_resources_enemysets(t_game *g)
{
	uint32_t	n;
	uint32_t	i;

	n = 0;
	while (n < sizeof(g->enemyset) / sizeof(*g->enemyset))
	{
		i = -1U;
		while (++i < sizeof(g->enemyset[n].walk) / sizeof(*g->enemyset[n].walk))
			free_image_array(g, g->enemyset[n].walk[i], \
            sizeof(g->enemyset[n].walk[i]) / sizeof(*g->enemyset[n].walk[i]));
		free_image_array(g, g->enemyset[n].wait, \
        sizeof(g->enemyset[n].wait) / sizeof(*g->enemyset[n].wait));
		free_image_array(g, g->enemyset[n].attack, \
        sizeof(g->enemyset[n].attack) / sizeof(*g->enemyset[n].attack));
		free_image_array(g, g->enemyset[n].pain, \
        sizeof(g->enemyset[n].pain) / sizeof(*g->enemyset[n].pain));
		free_image_array(g, g->enemyset[n].death, \
        sizeof(g->enemyset[n].death) / sizeof(*g->enemyset[n].death));
		i = 0;
		while (i < g->enemyset[n].s_death_count)
			cs_free_sound(&g->enemyset[n].s_death[i++].file);
		cs_free_sound(&g->enemyset[n].s_alarm.file);
		cs_free_sound(&g->enemyset[n].s_attack.file);
		n++;
	}
}

void	free_image_array(t_game *game, t_img *arr, uint32_t count)
{
	uint32_t	i;

	i = 0;
	while (i < count)
	{
		if (arr[i].ptr)
		{
			mlx_destroy_image(game->mlx, arr[i].ptr);
			free(arr[i].alpha_y);
		}
		i++;
	}
}

void	free_object(void *object)
{
	free(((t_object *)object)->e);
	free(object);
}

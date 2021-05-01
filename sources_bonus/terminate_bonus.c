/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 23:31:40 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 18:16:17 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int	terminate(t_game *game, int return_value, char *message)
{
	if (return_value)
	{
		ft_putendl_fd("Error", 2);
		ft_putendl_fd(message, 2);
		if (return_value == ERR_MLX)
			ft_putendl_fd("MLX/X11 crashed", 2);
		else if (return_value == ERR_PARSE)
			ft_putendl_fd("Plese fix scene file", 2);
		else if (return_value == ERR_ARGS)
			ft_putendl_fd("Usage: ./cub3D scene_name.cub [--save]", 2);
		else if (return_value == ERR_BMP)
			ft_putendl_fd("Unable to save screenshot", 2);
	}
	free_resources(game);
	if (game->map.img.ptr)
		mlx_destroy_image(game->mlx, game->map.img.ptr);
	if (game->img.ptr)
		mlx_destroy_image(game->mlx, game->img.ptr);
	if (game->img_effect.ptr)
		mlx_destroy_image(game->mlx, game->img_effect.ptr);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	terminate_audio(game);
	free(game->mlx);
	exit(return_value);
}

void	free_resources(t_game *g)
{
	uint32_t	i;

	ft_lstclear(&g->objects, terminate_free_object);
	free(g->column);
	free(g->angles);
	if (g->map.grid)
		ft_free_ptr_array((void **)g->map.grid, g->map.size.y);
	free_image_array(g, g->texture, sizeof(g->texture) / sizeof(*g->texture));
	free_image_array(g, g->sprite, sizeof(g->sprite) / sizeof(*g->sprite));
	i = -1U;
	while (++i < sizeof(g->p.weapon_img) / sizeof(*g->p.weapon_img))
		free_image_array(g, g->p.weapon_img[i], \
				sizeof(g->p.weapon_img[i]) / sizeof(*g->p.weapon_img[i]));
	i = -1U;
	while (++i < sizeof(g->enemyset[0].walk) / sizeof(*g->enemyset[0].walk))
		free_image_array(g, g->enemyset[0].walk[i], \
			sizeof(g->enemyset[0].walk[i]) / sizeof(*g->enemyset[0].walk[i]));
	free_image_array(g, g->enemyset[0].wait, \
		sizeof(g->enemyset[0].wait) / sizeof(*g->enemyset[0].wait));
	free_image_array(g, g->enemyset[0].attack, \
		sizeof(g->enemyset[0].attack) / sizeof(*g->enemyset[0].attack));
	free_image_array(g, g->enemyset[0].pain, \
		sizeof(g->enemyset[0].pain) / sizeof(*g->enemyset[0].pain));
	free_image_array(g, g->enemyset[0].death, \
		sizeof(g->enemyset[0].death) / sizeof(*g->enemyset[0].death));
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

void	terminate_free_object(void *object)
{
	free(((t_object *)object)->e);
	free(object);
}

void	terminate_audio(t_game *game)
{
	uint32_t	i;

	i = 0;
	while (i < sizeof(game->audio.music) / sizeof(*game->audio.music))
		cs_free_sound(&game->audio.music[i++].file);
	i = 0;
	while (i < sizeof(game->audio.sound) / sizeof(*game->audio.sound))
		cs_free_sound(&game->audio.sound[i++].file);
	i = 0;
	while (i < game->enemyset[0].s_death_count)
		cs_free_sound(&game->enemyset[0].s_death[i++].file);
	cs_free_sound(&game->enemyset[0].s_alarm.file);
	cs_free_sound(&game->enemyset[0].s_attack.file);
	if (game->audio.ctx)
		cs_shutdown_context(game->audio.ctx);
	if (game->audio.ctx7)
		cs_shutdown_context(game->audio.ctx7);
	if (game->audio.ctx22)
		cs_shutdown_context(game->audio.ctx22);
}

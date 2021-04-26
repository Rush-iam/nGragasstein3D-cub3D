/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 23:31:40 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/16 13:41:21 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int		terminate(t_game *game, int return_value, char *message)
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
	terminate_free(game);
	if (game->map.img.ptr)
		mlx_destroy_image(game->mlx, game->map.img.ptr);
	if (game->img.ptr)
		mlx_destroy_image(game->mlx, game->img.ptr);
	if (game->effect_img.ptr)
		mlx_destroy_image(game->mlx, game->effect_img.ptr);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	terminate_audio(game);
	free(game->mlx);
	exit(return_value);
}

void	terminate_free(t_game *game)
{
	unsigned	i;

	ft_lstclear(&game->objects, terminate_free_object);
	free(game->column);
	free(game->angles);
	if (game->map.grid && (i = 0) == 0)
		while (i < game->map.size.y)
			free(game->map.grid[i++]);
	free(game->map.grid);
	terminate_free_images(game, game->texture,
								sizeof(game->texture) / sizeof(*game->texture));
	terminate_free_images(game, game->sprite,
								sizeof(game->sprite) / sizeof(*game->sprite));
	i = -1U;
	while (++i < sizeof(game->p.weapon_img) / sizeof(*game->p.weapon_img))
		terminate_free_images(game, game->p.weapon_img[i],
				sizeof(game->p.weapon_img[i]) / sizeof(*game->p.weapon_img[i]));
	terminate_free_images(game, game->enemyset[0].wait,
			sizeof(game->enemyset[0].wait) / sizeof(*game->enemyset[0].wait));
	terminate_free_images(game, game->enemyset[0].attack,
			sizeof(game->enemyset[0].attack) / sizeof(*game->enemyset[0].attack));
	terminate_free_images(game, game->enemyset[0].pain,
			sizeof(game->enemyset[0].pain) / sizeof(*game->enemyset[0].pain));
	terminate_free_images(game, game->enemyset[0].death,
			sizeof(game->enemyset[0].death) / sizeof(*game->enemyset[0].death));
}

void	terminate_free_images(t_game *game, t_img *arr, unsigned count)
{
	unsigned	i;

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
	unsigned	i;

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
	cs_shutdown_context(game->audio.ctx);
	cs_shutdown_context(game->audio.ctx7);
	cs_shutdown_context(game->audio.ctx22);
}
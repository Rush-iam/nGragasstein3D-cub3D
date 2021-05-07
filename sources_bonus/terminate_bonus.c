/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 23:31:40 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/07 12:52:13 by ngragas          ###   ########.fr       */
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
			ft_putendl_fd("Usage: ./cub3D scene_name.cub [--save]\n" \
							"Additional info: ./cub3D --help", 2);
		else if (return_value == ERR_BMP)
			ft_putendl_fd("Unable to save screenshot", 2);
	}
	free_resources(game);
	free_resources_enemysets(game);
	terminate_engine_hud(game);
	terminate_engine(game);
	exit(return_value);
}

void	terminate_help(t_game *game)
{
	ft_putendl_fd("================ nGragasstein 3D ================", 1);
	ft_putendl_fd("= inspired by Wolfenstein 3D game (id Software) =", 1);
	ft_putendl_fd("=================================================", 1);
	ft_putendl_fd("===========       Game controls       ===========", 1);
	ft_putendl_fd("= WASD - move                                   =", 1);
	ft_putendl_fd("= Mouse (Arrows) - rotate                       =", 1);
	ft_putendl_fd("= Mouse Left (RightControl) - attack            =", 1);
	ft_putendl_fd("= Mouse Right / Mouse Middle - zoom-in/out      =", 1);
	ft_putendl_fd("= E - open door/push                            =", 1);
	ft_putendl_fd("= M - minimap                                   =", 1);
	ft_putendl_fd("= Backspace - release/lock mouse pointer        =", 1);
	ft_putendl_fd("= + / - / * - change Field Of View              =", 1);
	ft_putendl_fd("= Space / LeftControl - jump / crouch           =", 1);
	ft_putendl_fd("=================================================", 1);
	ft_putendl_fd("= Have fun! For any bugs and feedback:          =", 1);
	ft_putendl_fd("============== rushiam@gmail.com ================", 1);
	ft_putendl_fd("=================================================", 1);
	free(game->mlx);
	exit(EXIT_SUCCESS);
}

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

void	terminate_engine(t_game *game)
{
	if (game->audio.ctx)
		cs_shutdown_context(game->audio.ctx);
	if (game->audio.ctx7)
		cs_shutdown_context(game->audio.ctx7);
	if (game->audio.ctx22)
		cs_shutdown_context(game->audio.ctx22);
	free(game->column);
	free(game->angles);
	if (game->map.img.ptr)
		mlx_destroy_image(game->mlx, game->map.img.ptr);
	if (game->img.ptr)
		mlx_destroy_image(game->mlx, game->img.ptr);
	if (game->img_effect.ptr)
		mlx_destroy_image(game->mlx, game->img_effect.ptr);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	free(game->mlx);
}

void	terminate_engine_hud(t_game *game)
{
	uint32_t	i;

	if (game->hud.digits.ptr)
		mlx_destroy_image(game->mlx, game->hud.digits.ptr);
	if (game->hud.bar.ptr)
		mlx_destroy_image(game->mlx, game->hud.bar.ptr);
	i = 0;
	while (i < 3)
	{
		free_image_array(game, game->hud.face[i], \
                        sizeof(game->hud.face[i]) / sizeof(*game->hud.face[i]));
		i++;
	}
}
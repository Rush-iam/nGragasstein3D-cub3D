/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 23:31:40 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/19 21:20:33 by ngragas          ###   ########.fr       */
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
	terminate_sound(game);
	free_resources(game);
	free_resources_enemysets(game);
	terminate_engine_hud(game);
	terminate_engine(game);
	exit(return_value);
}

void	terminate_sound(t_game *game)
{
	if (game->audio.ctx)
		cs_shutdown_context(game->audio.ctx);
	if (game->audio.ctx7)
		cs_shutdown_context(game->audio.ctx7);
	if (game->audio.ctx22)
		cs_shutdown_context(game->audio.ctx22);
}

void	terminate_engine(t_game *game)
{
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

void	print_help(t_game *game)
{
	ft_putendl_fd("================ nGragasstein 3D ================", 1);
	ft_putendl_fd("= inspired by Wolfenstein 3D game (id Software) =", 1);
	ft_putendl_fd("=================================================", 1);
	ft_putendl_fd("===========       Game controls       ===========", 1);
	ft_putendl_fd("=                   WASD - move                 =", 1);
	ft_putendl_fd("=         Mouse (Arrows) - rotate               =", 1);
	ft_putendl_fd("= Mouse Left (RightCtrl) - attack               =", 1);
	ft_putendl_fd("=   Mouse Right / Middle - zoom-in/out          =", 1);
	ft_putendl_fd("=                      E - open door/push       =", 1);
	ft_putendl_fd("=                      M - minimap              =", 1);
	ft_putendl_fd("=              Backspace - release/lock mouse   =", 1);
	ft_putendl_fd("=              + / - / * - change Field Of View =", 1);
	ft_putendl_fd("=                  Shift - run                  =", 1);
	ft_putendl_fd("=       Space / LeftCtrl - jump / crouch        =", 1);
	ft_putendl_fd("=================================================", 1);
	ft_putendl_fd("=====  Have fun! For any bugs and issues:   =====", 1);
	ft_putendl_fd("=== github.com/Rush-iam/nGragasstein3D-cub3D  ===", 1);
	ft_putendl_fd("=================================================", 1);
	free(game->mlx);
	exit(EXIT_SUCCESS);
}

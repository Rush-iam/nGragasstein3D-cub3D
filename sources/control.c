/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 20:55:59 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/22 21:03:45 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	player_control(t_game *game)
{
	if (game->key.k[TURN_LEFT])
		game->p.angle -= PL_SPEED / 2;
	if (game->key.k[TURN_RIGHT])
		game->p.angle += PL_SPEED / 2;
	if (game->key.k[MOVE_FORWARD])
		game->p.pos = (t_fpoint){game->p.pos.x + PL_SPEED * cos(game->p.angle),
								game->p.pos.y + PL_SPEED * sin(game->p.angle)};
	if (game->key.k[MOVE_BACK])
		game->p.pos = (t_fpoint){game->p.pos.x - PL_SPEED * cos(game->p.angle),
								game->p.pos.y - PL_SPEED * sin(game->p.angle)};
	if (game->key.k[MOVE_LEFT])
		game->p.pos = (t_fpoint){game->p.pos.x + PL_SPEED * sin(game->p.angle),
								game->p.pos.y - PL_SPEED * cos(game->p.angle)};
	if (game->key.k[MOVE_RIGHT])
		game->p.pos = (t_fpoint){game->p.pos.x - PL_SPEED * sin(game->p.angle),
								game->p.pos.y + PL_SPEED * cos(game->p.angle)};
	player_control_borders(game);
}

void	player_control_borders(t_game *game)
{
	if (game->p.angle > PI2)
		game->p.angle -= PI2;
	if (game->p.angle < 0)
		game->p.angle += PI2;
	if (game->p.pos.x < 0)
		game->p.pos.x = 0.0001;
	if (game->p.pos.y < 0)
		game->p.pos.y = 0.0001;
	if (game->p.pos.x >= game->map.size.x)
		game->p.pos.x = game->map.size.x - 0.0001;
	if (game->p.pos.y >= game->map.size.y)
		game->p.pos.y = game->map.size.y - 0.0001;
}

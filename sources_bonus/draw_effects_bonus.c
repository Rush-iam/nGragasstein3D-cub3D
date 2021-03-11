/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_effects_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 18:14:51 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/11 18:14:51 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_effect(t_game *game, struct s_effect *ef)
{
	float	power;

	if (ef->frame_cur < ef->frames)
	{
		if (ef->frame_cur >= ef->frames)
			ef->frame_cur = ef->frames - 1;
		power = (float)ef->frame_cur / (ef->frames / 2.);
		if (power > 1)
			power = 2 - power;
		if (ef->type == EF_FLASH)
			effect_flash(game, game->effect.color, power * ef->max_power);
	}
}

void	effect_flash(t_game *game, unsigned color, float power)
{
	img_clear_rgb(&game->effect_img, color | ((int)(255 - 255. * power) << 24));
	mlx_put_image_to_window(game->mlx, game->win, game->effect_img.ptr, 0, 0);
}

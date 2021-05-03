/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_effects_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 18:14:51 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 15:00:34 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_effect(t_game *game, struct s_effect *ef)
{
	float	power;

	if (ef->frame_cur < ef->frames)
	{
		power = (float)ef->frame_cur / (ef->frames / 2.0f);
		if (power > 1.0f)
			power = 2.0f - power;
		if (ef->type == EF_FLASH)
			effect_flash(game, game->effect.color, power * ef->max_power);
		else if (ef->type == EF_FIZZLEFADE)
			effect_fizzlefade(game, game->effect.color);
	}
}

void	effect_flash(t_game *game, uint32_t color, float power)
{
	img_clear_rgb(&game->img_effect, color | ((int)(255 - 255 * power) << 24));
	mlx_put_image_to_window(game->mlx, game->win, game->img_effect.ptr, 0, 0);
}

void	effect_fizzlefade(t_game *game, uint32_t color)
{
	static uint32_t	rndval = 1;
	uint32_t		lsb;
	t_point			pos;
	uint32_t		i;
	uint32_t		scale;

	scale = ft_max(game->center.x, game->img.size.y) / 256 + 1;
	img_clear_rgb(&game->img_effect, 0xFF000000);
	i = 0;
	while (i < 1024)
	{
		pos.x = ((rndval & 0x1FF00) >> 8) * scale;
		pos.y = (rndval & 0x000FF) * scale;
		lsb = rndval & 1;
		rndval >>= 1;
		if (lsb)
			rndval ^= 0x12000;
		draw_square_fill(&game->img_effect, pos, scale, color);
		i++;
	}
	mlx_put_image_to_window(game->mlx, game->win, game->img_effect.ptr, 0, 0);
}

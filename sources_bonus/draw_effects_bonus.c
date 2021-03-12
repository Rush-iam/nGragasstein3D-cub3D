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

void	effect_fizzlefade(t_img *img, unsigned color)
{
	static unsigned	rndval = 1;
	static int		frames;
	unsigned		lsb;
	t_upoint		pos;
	int				i;

	if (frames == 256)
		return ;
	frames++;
	i = 0;
	while (i++ < 512)
	{
		pos.x = ((rndval & 0x1FF00) >> 8) * 2;
		pos.y = (rndval & 0x000FF) * 2;
		lsb = rndval & 1;
		rndval >>= 1;
		if (lsb)
			rndval ^= 0x12000;
		pixel_put(img, pos.x, pos.y, color);
		pixel_put(img, pos.x + 1, pos.y, color);
		pixel_put(img, pos.x, pos.y + 1, color);
		pixel_put(img, pos.x + 1, pos.y + 1, color);
	}
}

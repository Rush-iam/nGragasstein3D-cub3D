/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_message_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 18:31:29 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 18:31:29 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	message_add(t_game *g, char *string, int timer, uint32_t color)
{
	ft_strlcpy(g->string.text, string, MAX_MESSAGE_LEN + 1);
	g->string.pos.x = g->img.size.x / 2 - \
						ft_umin(ft_strlen(string), MAX_MESSAGE_LEN) * 7;
	g->string.pos.y = g->img.size.y / 2 + 14;
	if (g->img.size.x == 0)
		g->string.pos.x = 0;
	g->string.frame_cur = 0;
	g->string.frames = timer * TICKS_PER_SEC;
	g->string.fade = (timer >= 3);
	g->string.color = color;
}

void	draw_message(t_game *g, struct s_string *s)
{
	float	fade;

	if (s->pos.x == 0)
		g->string.pos = (t_upoint){\
		g->img.size.x / 2 - ft_umin(ft_strlen(s->text), MAX_MESSAGE_LEN) * 7, \
		g->img.size.y / 2 + 14};
	if (s->frame_cur < s->frames)
	{
		fade = 1.0f;
		if (s->fade == true)
		{
			if (s->frame_cur < TICKS_PER_SEC)
				fade = (float)s->frame_cur / TICKS_PER_SEC;
			else if (s->frames - s->frame_cur < TICKS_PER_SEC)
				fade = (float)(s->frames - s->frame_cur) / TICKS_PER_SEC;
		}
		mlx_string_put(g->mlx, g->win, s->pos.x, s->pos.y, \
					   pixel_fade(s->color, fade), s->text);
	}
	else
		s->text[0] = '\0';
}

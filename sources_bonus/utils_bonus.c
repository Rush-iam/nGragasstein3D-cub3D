/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:41 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/28 15:18:53 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

char	*atoi_limited(unsigned *dst_int, const char *src_string, unsigned limit)
{
	unsigned long	num;

	while (*src_string == ' ')
		src_string++;
	if (ft_isdigit(*src_string) == false)
		return (NULL);
	num = 0;
	while (ft_isdigit(*src_string))
	{
		num = num * 10 + *src_string++ - '0';
		if (limit != UINT_MAX && num > limit)
			return (NULL);
		if (num > UINT_MAX)
			while (ft_isdigit(*src_string))
				src_string++;
	}
	*dst_int = ft_umin(num, UINT_MAX);
	while (*src_string == ' ')
		src_string++;
	return ((char *)src_string);
}

void	string_add(t_game *g, char *string, int timer, unsigned color)
{
	free(g->string.text);
	g->string.text = string;
	g->string.pos.x = g->img.size.x / 2 - ft_strlen(string) * 7;
	g->string.pos.y = g->img.size.y / 2 + 14;
	if (g->img.size.x == 0)
		g->string.pos.x = 0;
	g->string.frame_cur = 0;
	g->string.frames = timer * TICKS_PER_SEC;
	g->string.fade = (timer >= 3);
	g->string.color = color;
}

void	draw_string(t_game *g, struct s_string *s)
{
	float	fade;

	if (s->pos.x == 0)
	{
		g->string.pos.x = g->img.size.x / 2 - ft_strlen(s->text) * 7;
		g->string.pos.y = g->img.size.y / 2 + 14;
	}
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
		mlx_string_put(g->mlx, g->win, s->pos.x, s->pos.y,
					   pixel_fade(s->color, fade), s->text);
	}
	else
	{
		free(s->text);
		s->text = NULL;
	}
}
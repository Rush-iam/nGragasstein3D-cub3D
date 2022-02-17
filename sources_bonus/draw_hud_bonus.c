/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_hud_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 15:57:36 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 15:03:24 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_hud(t_game *g)
{
	char		num_str[11];
	const float	y_offset = g->hud.bar.size.y * HUD_VALUES_OFFSET_Y;

	if (g->hud.needs_redraw == true)
	{
		hud_digits_bake(&g->hud, hud_num_to_str(num_str, g->p.score, 6), \
						(t_upoint){g->hud.bar.size.x * HUD_SCORE_X, y_offset});
		hud_digits_bake(&g->hud, hud_num_to_str(num_str, 1, 1), \
						(t_upoint){g->hud.bar.size.x * HUD_FLOOR_X, y_offset});
		hud_digits_bake(&g->hud, hud_num_to_str(num_str, 1, 1), \
						(t_upoint){g->hud.bar.size.x * HUD_LIVES_X, y_offset});
		hud_digits_bake(&g->hud, hud_num_to_str(num_str, g->p.health, 3), \
						(t_upoint){g->hud.bar.size.x * HUD_HEALTH_X, y_offset});
		hud_digits_bake(&g->hud, hud_num_to_str(num_str, g->p.ammo, 2), \
						(t_upoint){g->hud.bar.size.x * HUD_AMMO_X, y_offset});
	}
	put_image_to_window(g->mlx, g->win, \
		g->hud.bar.ptr, (t_point){0, g->img.size.y});
	put_image_to_window(g->mlx, g->win, \
		g->hud.weapon[g->p.weapon_cur].ptr, \
		(t_point){g->hud.bar.size.x * HUD_WEAPON_X, \
		g->img.size.y + g->hud.bar.size.y * HUD_WEAPON_Y});
	draw_hud_face(g, true);
	g->hud.needs_redraw = false;
}

void	draw_hud_face(t_game *g, bool force_redraw)
{
	int			health_level;
	static int	face_dir;

	if (g->p.health == 0)
	{
		put_image_to_window(g->mlx, g->win, g->hud.face_dead.ptr, \
							g->hud.face_pos);
		return ;
	}
	if (g->hud.face_nexttick < g->tick)
	{
		face_dir = random() % HUD_FACE_DIRS;
		g->hud.face_nexttick = g->tick + HUD_FACE_TICK_TIMER;
		force_redraw = true;
	}
	if (force_redraw == false)
		return ;
	health_level = (105 - g->p.health) / 15;
	if (health_level < 0)
		health_level = 0;
	else if (health_level >= HUD_FACE_LEVELS)
		health_level = HUD_FACE_LEVELS - 1;
	put_image_to_window(g->mlx, g->win, \
						g->hud.face[health_level][face_dir].ptr, \
						g->hud.face_pos);
}

char	*hud_num_to_str(char num_str[11], uint32_t num, int length)
{
	if (length == 0)
		return (num_str);
	num_str[length] = '\0';
	num_str[length - 1] = '0';
	if (num == 0)
		length--;
	while (num && length--)
	{
		num_str[length] = '0' + num % 10;
		num /= 10;
	}
	while (length-- > 0)
		num_str[length] = ':';
	return (num_str);
}

void	hud_digits_bake(struct s_hud *hud, char *num_str, t_upoint pos)
{
	int			i;
	int			src_x_offset;
	t_upoint	src_px;

	i = 0;
	while (num_str[i])
	{
		src_x_offset = (num_str[i] - '0') * hud->digit_width + 1;
		src_px.y = 0;
		while (src_px.y < hud->digits.size.y)
		{
			src_px.x = 0;
			while (src_px.x < (uint32_t)hud->digit_width)
			{
				hud->bar.data[hud->bar.size.x * (pos.y + src_px.y) + \
							pos.x + src_px.x + i * (int)hud->digit_width] = \
					hud->digits.data[src_px.y * hud->digits.size.x + \
											src_x_offset + src_px.x];
				src_px.x++;
			}
			src_px.y++;
		}
		i++;
	}
}

void	draw_fps(t_game *game)
{
	static clock_t	clock_prev;
	const clock_t	clock_cur = clock();
	int				fps;

	if (clock_cur != clock_prev)
	{
		fps = CLOCKS_PER_SEC / (clock_cur - clock_prev);
		clock_prev = clock_cur;
		mlx_string_put(game->mlx, game->win, 0, 20, COLOR_WHITE, \
        (char[]){'0' + fps / 100, '0' + fps / 10 % 10, '0' + fps % 10, '\0'});
	}
}

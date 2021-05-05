/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ceilfloor_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/30 19:26:18 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/05 16:20:37 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_ceil_textured(t_game *g, t_point px)
{
	const t_fpoint	angle = {g->ray_vector.x * g->col_scale,
							g->ray_vector.y * g->col_scale};
	float			height;
	t_fpoint		pt;
	const t_img		src = g->texture_ceil;

	height = g->column[px.x].height + g->height_step_up;
	if (px.y > (int)g->img.size.y)
	{
		height += (px.y - g->img.size.y) * g->height_step_up;
		px.y = g->img.size.y;
	}
	if (g->col_scale / height > g->fade_distance)
		px.y = draw_ceil_texture_faded(g, px, &height);
	while (px.y--)
	{
		pt = (t_fpoint){
			fabsf((g->p.pos.x + angle.x / height) / TEXTURE_CEIL_CELLSCALE),
			fabsf((g->p.pos.y + angle.y / height) / TEXTURE_CEIL_CELLSCALE)};
		pt = (t_fpoint){pt.x - (int)pt.x, pt.y - (int)pt.y};
		g->img.data[px.y * g->img.size.x + px.x] = src.data[
			src.size.x * (int)(pt.y * src.size.y) + (int)(pt.x * src.size.x)];
		height += g->height_step_up;
	}
}

int	draw_ceil_texture_faded(t_game *g, t_point px, float *height)
{
	const t_fpoint	angle = g->ray_vector;
	float			dist;
	t_fpoint		pt;
	const t_img		src = g->texture_ceil;

	dist = g->col_scale / *height;
	while (px.y && (int)dist >= g->fade_distance)
	{
		px.y--;
		pt = (t_fpoint){\
				fabsf(g->p.pos.x + angle.x * dist) / TEXTURE_CEIL_CELLSCALE, \
				fabsf(g->p.pos.y + angle.y * dist) / TEXTURE_CEIL_CELLSCALE};
		pt = (t_fpoint){pt.x - (int)pt.x, pt.y - (int)pt.y};
		g->img.data[px.y * g->img.size.x + px.x] = pixel_fade(src.data[\
			src.size.x * (int)(pt.y * src.size.y) + \
			(int)(pt.x * src.size.x)], g->fade_distance / dist);
		*height += g->height_step_up;
		dist = g->col_scale / *height;
	}
	return (px.y);
}

void	draw_floor_textured(t_game *g, t_point px)
{
	const t_fpoint	angle = {g->ray_vector.x * g->col_scale,
							g->ray_vector.y * g->col_scale};
	float			height;
	t_fpoint		pt;
	const t_img		src = g->texture_floor;

	height = g->column[px.x].height;
	if (px.y < 0)
	{
		height -= px.y * g->height_step_down;
		px.y = 0;
	}
	if (g->col_scale / height > g->fade_distance)
		px.y = draw_floor_texture_faded(g, px, &height);
	while (px.y < (int)g->img.size.y)
	{
		height += g->height_step_down;
		pt = (t_fpoint){\
			fabsf(g->p.pos.x + angle.x / height) / TEXTURE_FLOOR_CELLSCALE, \
			fabsf(g->p.pos.y + angle.y / height) / TEXTURE_FLOOR_CELLSCALE};
		pt = (t_fpoint){pt.x - (int)pt.x, pt.y - (int)pt.y};
		g->img.data[px.y * g->img.size.x + px.x] = src.data[
			src.size.x * (int)(pt.y * src.size.y) + (int)(pt.x * src.size.x)];
		px.y++;
	}
}

int	draw_floor_texture_faded(t_game *g, t_point px, float *height)
{
	const t_fpoint	angle = g->ray_vector;
	float			dist;
	t_fpoint		pt;
	const t_img		src = g->texture_floor;

	dist = g->col_scale / *height;
	while (px.y < (int)g->img.size.y && (int)dist >= g->fade_distance)
	{
		pt = (t_fpoint){\
			fabsf(g->p.pos.x + angle.x * dist) / TEXTURE_FLOOR_CELLSCALE, \
			fabsf(g->p.pos.y + angle.y * dist) / TEXTURE_FLOOR_CELLSCALE};
		pt = (t_fpoint){pt.x - (int)pt.x, pt.y - (int)pt.y};
		g->img.data[px.y * g->img.size.x + px.x] = pixel_fade(src.data[\
			src.size.x * (int)(pt.y * src.size.y) + \
			(int)(pt.x * src.size.x)], g->fade_distance / dist);
		*height += g->height_step_down;
		dist = g->col_scale / *height;
		px.y++;
	}
	return (px.y);
}

//void	draw_ceilfloor_textured_fill(t_game *g)
//{
//	t_point			px;
//	t_fpoint		pt;
//	t_fpoint		ray_vector;
//	t_fpoint		mult_angle;
//	float			height;
//	const t_img		src = g->texture[TEXTURE_CEIL];
//
//	if (g->horizon <= 0)
//		return ;
//	g->height_step_up = 1.0f / (0.5f - g->z_level);
//	g->height_step_down = 1.0f / (0.5f + g->z_level);
//	px.y = g->horizon;
//	height = g->height_step_up;
//	if (px.y > (int)g->img.size.y)
//	{
//		height += (px.y - g->img.size.y) * g->height_step_up;
//		px.y = g->img.size.y;
//	}
//	while (px.y--)
//	{
//		px.x = 0;
//		while (px.x < (int)g->img.size.x)
//		{
//		__sincosf(g->p.angle + g->angles[px.x], &ray_vector.y, &ray_vector.x);
//			ray_vector.x /= cosf(g->angles[px.x]);
//			ray_vector.y /= cosf(g->angles[px.x]);
//			mult_angle = (t_fpoint){ray_vector.x * g->col_scale,
//									ray_vector.y * g->col_scale};
//			pt = (t_fpoint){fabsf(g->p.pos.x + mult_angle.x / height),
//							fabsf(g->p.pos.y + mult_angle.y / height)};
//			g->img.data[px.y * g->img.size.x + px.x] = src.data[
//					(int)(((pt.y - (int)pt.y)) * src.size.y) * src.size.x +
//					(int)((pt.x - (int)pt.x) * src.size.x)];
//			px.x++;
//		}
//		height += g->height_step_up;
//	}
//}

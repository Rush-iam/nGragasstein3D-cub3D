/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:57:18 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/05 22:25:44 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <mlx.h>
# include <math.h>
# include <time.h>
# include "libft.h"

# define WIN_W 1280
# define WIN_H 720

# define COLOR_CEIL 60
# define COLOR_FLOOR 105

typedef struct	s_point
{
	int	x;
	int	y;
}				t_point;

typedef struct	s_img
{
	void	*ptr;
	int		*data;
	t_point	size;
}				t_img;

typedef struct	s_mlx
{
	void	*mlx;
	void	*win;
	t_img	buf;
}				t_mlx;

int				game_loop		(t_mlx *mlx);
int				terminate		(void);

int				hook_key_press		(int key, t_mlx *mlx);
int				hook_key_release	(int key, t_mlx *mlx);
int				hook_mouse_press	(int button, int x, int y, t_mlx *mlx);
int				hook_mouse_release	(int button, int x, int y, t_mlx *mlx);
int				hook_mouse_move		(int x, int y, t_mlx *mlx);

void			img_clear				(t_img *img);
void			img_clear_rgb			(t_img *img, int color);
void			img_ceilfloor_fill		(t_img *img, unsigned char ceil,
													unsigned char floor);
void			img_ceilfloor_fill_rgb	(t_img *img, int ceil, int floor);

void			pixel_put	(t_img *img, int x, int y, int color);
void			draw_line	(t_img *img, t_point p1, t_point p2, int color);

void			demo_fillrate	(t_mlx *mlx, int step);
void			demo_radar		(t_mlx *mlx, int rays);

#endif

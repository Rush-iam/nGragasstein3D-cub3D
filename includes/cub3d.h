/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:57:18 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/07 20:39:52 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <time.h>
# include <math.h>
# include <stdbool.h>
# include <mlx.h>
# include "libft.h"
# include "x_events.h"

# define WIN_W 1200 * 2
# define WIN_H 672 * 2

# define COLOR_CEIL		60
# define COLOR_FLOOR	105

# define PI2			(2 * M_PI)
# define GRAD_TO_RAD	(1 / 360. * PI2)

# define MOVE_FORWARD	KEY_W
# define MOVE_BACK		KEY_S
# define MOVE_LEFT		KEY_A
# define MOVE_RIGHT		KEY_D
# define TURN_LEFT		KEY_LEFT
# define TURN_RIGHT		KEY_RIGHT

# define MAP_SCALE		32
# define COL_SCALE		WIN_H
# define PLAYER_SPEED	0.05
# define POV_ANGLE		60.

typedef struct	s_point
{
	signed int	x;
	signed int	y;
}				t_point;

typedef struct	s_upoint
{
	unsigned	x;
	unsigned	y;
}				t_upoint;

typedef struct	s_fpoint
{
	double		x;
	double		y;
}				t_fpoint;

typedef struct	s_img
{
	void		*ptr;
	int			*data;
	t_upoint	size;
}				t_img;

typedef struct	s_game
{
	void		*mlx;
	void		*win;
	t_img		img;
	struct		s_player
	{
		t_fpoint	pos;
		float		angle;
	}			p;
	struct		s_key
	{
		bool		k[280];
		bool		m[10];
		t_point		mpos;
		t_point		mdir;
	}			key;
	struct		s_map
	{
		t_upoint	size;
		char		*grid[14];
	}			map;
	struct		s_column
	{
		unsigned	height;
		double		distance;
		t_fpoint	cell;
		char		dir;
	}			column[WIN_W];
}				t_game;

int				game_loop		(t_game *game);
int				terminate		(void);

int				hook_key_press		(int key, struct s_key *input);
int				hook_key_release	(int key, struct s_key *input);
int				hook_mouse_press	(int button, int x, int y, struct s_key *input);
int				hook_mouse_release	(int button, int x, int y, struct s_key *input);
int				hook_mouse_move		(int x, int y, struct s_key *input);

void			img_clear				(t_img *img);
void			img_clear_rgb			(t_img *img, int color);
void			img_ceilfloor_fill		(t_img *img, unsigned char ceil,
												unsigned char floor);
void			img_ceilfloor_fill_rgb	(t_img *img, int ceil, int floor);
void			fizzlefade				(t_img *img, int color);

void			pixel_put	(t_img *img, unsigned x, unsigned y, int color);
void			draw_line	(t_img *img, t_point p1, t_point p2, int color);
void			draw_square(t_img *img, t_point center, int size, int color);
void			draw_4pts	(t_img *img, t_point *pts, int color);

t_point			points_sum	(t_point p1, t_point p2);

void			demo_fillrate	(t_game *mlx, int step);
void			demo_radar		(t_game *mlx, int rays);
void			demo_cursor		(t_game *game, int color);

#endif

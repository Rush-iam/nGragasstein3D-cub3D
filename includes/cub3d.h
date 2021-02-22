/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:57:18 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/22 23:22:06 by ngragas          ###   ########.fr       */
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

# define ERROR_ARGS		1
# define ERROR_PARSE	2
# define ERROR_MEMORY	3

# define WIN_W (1200 * 2)
# define WIN_H (600 * 2)

# define COLOR_CEIL		0x201010
# define COLOR_FLOOR	0x503020

# define WALL_N	0
# define WALL_S	1
# define WALL_W	2
# define WALL_E	3

# define MOVE_FORWARD	KEY_W
# define MOVE_BACK		KEY_S
# define MOVE_LEFT		KEY_A
# define MOVE_RIGHT		KEY_D
# define TURN_LEFT		KEY_LEFT
# define TURN_RIGHT		KEY_RIGHT

# define PI2			(2 * M_PI)
# define GRAD_TO_RAD	(PI2 / 360)
# define MAP_SCALE		32
# define FOV			(60. * GRAD_TO_RAD)
/*
** tan(FOV / 2) if FOV 60 == .57735
*/
# define TAN_60_2		.57735
# define COL_SCALE		(2 * TAN_60_2)
# define PL_SPEED		0.05

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

typedef struct	s_sprite
{
	t_img		img;
	double		aspect;
	bool		animated;
	unsigned	frames;
}				t_sprite;

typedef struct	s_object
{
	t_sprite	*sprite;
	double		width;
	unsigned	height;
	double		distance;
	t_fpoint	pos;
}				t_object;

typedef struct	s_game
{
	void		*mlx;
	void		*win;
	t_img		img;
	struct		s_player
	{
		t_fpoint	pos;
		double		angle;
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
		t_img		img;
		t_upoint	size;
		char		**grid;
	}			map;
	struct		s_column
	{
		double		distance;
		unsigned	height;
		t_fpoint	cell;
		double		texture_pos;
		char		dir;
	}			column[WIN_W];
	t_img		wall[4];
	t_sprite	sprite[1];
	t_object	object[4];
	unsigned	object_count;
}				t_game;

int				game_loop		(t_game *game);

int				hook_key_press		(int key_code, struct s_key *key);
int				hook_key_release	(int key_code, struct s_key *key);
int				hook_mouse_press	(int btn, int x, int y, struct s_key *key);
int				hook_mouse_release	(int btn, int x, int y, struct s_key *key);
int				hook_mouse_move		(int x, int y, struct s_key *key);

void			player_control			(t_game *game);
void			player_control_borders	(t_game *game);

void			ray_cast		(t_game *game);
void			ray_intersect	(t_game *game, double cur_angle, unsigned ray);
t_fpoint		ray_intersect_x	(t_game *game, t_fpoint step);
t_fpoint		ray_intersect_y	(t_game *game, t_fpoint step);

void			img_clear				(t_img *img);
void			img_clear_rgb			(t_img *img, int color);
void			img_ceilfloor_fill		(t_img *img, unsigned char ceil,
										unsigned char floor);
void			img_ceilfloor_fill_rgb	(t_img *img, int ceil, int floor);
void			fizzlefade				(t_img *img, int color);

void			pixel_put	(t_img *img, unsigned x, unsigned y, int color);
int				pixel_fade	(int color, double fade);
void			draw_line	(t_img *img, t_point p1, t_point p2, int color);
void			draw_square	(t_img *img, t_point center, int size, int color);
void			draw_4pts	(t_img *img, t_point *pts, int color);

void			draw_map(t_game *game);
void			draw_map_player(t_game *game);

void			draw_walls		(t_game *game);
void			draw_wall_scaled(t_game *game, t_img *src, unsigned x,
						double fade);
void			draw_wall_solid	(t_game *game, unsigned ray, double fade);

void			draw_objects		(t_game *game);
void			objects_sort		(t_game *game);
void			draw_sprite			(t_game *game, t_object *obj, double angle);
void			draw_sprite_scaled	(t_game *game, t_object *obj, unsigned x,
																unsigned src_x);

t_point			points_sum	(t_point p1, t_point p2);
int				terminate	(int return_value, char *message);

void			demo_fillrate	(t_game *mlx, int step);
void			demo_radar		(t_game *mlx, int rays);
void			demo_cursor		(t_game *game, int color);

#endif

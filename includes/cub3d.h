/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:57:18 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/26 17:14:16 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <errno.h>
# include <string.h>
# include <limits.h>
# include "mlx.h"
# include "libft.h"
# include "get_next_line.h"
# include "x_events.h"

# define WIN_TITLE	"cub3D by nGragas"
# define MAX_WIN	(t_upoint){2560, 1440}
# define MAX_SCR	(t_upoint){20000, 20000}
# define MIN_RES_X	2

# define ERR_MLX	1
# define ERR_ARGS	2
# define ERR_PARSE	3
# define ERR_MEM	4
# define ERR_BMP	5

# define WALL_N	0
# define WALL_S	1
# define WALL_W	2
# define WALL_E	3
# define SPRITE 4

# define K_MOVE_FORWARD	KEY_W
# define K_MOVE_BACK	KEY_S
# define K_MOVE_LEFT	KEY_A
# define K_MOVE_RIGHT	KEY_D
# define K_TURN_LEFT	KEY_LEFT
# define K_TURN_RIGHT	KEY_RIGHT

# define K_FOV_WIDE		KEY_NUMMINUS
# define K_FOV_TELE		KEY_NUMPLUS
# define K_FOV_RESET	KEY_NUMASTERISK

# define PL_SPEED	0.05
# define PI2		(2 * M_PI)

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
	unsigned	*data;
	t_upoint	size;
	double		aspect;
}				t_img;

typedef struct	s_object
{
	t_img		*sprite;
	t_fpoint	pos;
	t_upoint	size;
	double		distance;
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
		t_fpoint	vect;
	}			p;
	bool		key[280];
	struct		s_map
	{
		t_upoint	size;
		char		**grid;
	}			map;
	float		fov;
	float		col_center;
	double		col_step;
	double		col_scale;
	struct		s_column
	{
		double		distance;
		unsigned	height;
		double		texture_pos;
		char		dir;
	}			**column;
	unsigned	color_ceil;
	unsigned	color_floor;
	t_img		texture[5];
	t_list		*objects;
}				t_game;

void			initialize_game		(t_game *game, bool screenshot_only);
void			initialize_game_2	(t_game *game);
void			player_set_fov				(t_game *game, float fov, bool reset);
int				game_loop			(t_game *game);

void			parse(int args, char **av, t_game *game, bool *screenshot_only);
void			parse_scene(int file_id, char **line, t_game *game);
void			parse_map(int file_id, char *line, t_list *map, t_game *game);
void			validate_settings(t_game *game);

void			set_resolution		(const char *res_string, t_upoint *res,
															t_game *game);
void			set_colors			(const char *color_string, unsigned *target,
															t_game *game);
void			set_textures		(char *string, t_game *game);
void			set_textures_import(char *path, t_img *dst_img, t_game *game);

void			set_map				(t_game *game, t_list *map);
void			set_map_process		(t_game *game);
void			set_map_check_cell	(t_game *game, char **map, t_upoint pt);

void			player_control			(t_game *game);
void			player_control_borders	(t_game *g);
int				hook_key_press		(int key_code, t_game *game);
int				hook_key_release	(int key_code, t_game *game);
int				hook_exit			(t_game *game);

void			ray_cast		(t_game *game);
void			ray_intersect	(t_game *game, double cur_angle, unsigned ray);
t_fpoint		ray_intersect_x	(t_game *game, t_fpoint step);
t_fpoint		ray_intersect_y	(t_game *game, t_fpoint step);

void			img_ceilfloor_fill_rgb	(t_img *img, int ceil, int floor);
void			draw_walls				(t_game *g);
void			draw_wall_scaled		(t_game *game, t_img *src, unsigned x);

void			draw_objects		(t_game *g);
int				objects_sort		(t_object *obj1, t_object *obj2);
void			draw_sprite			(t_game *game, t_object *obj, double angle);
void			draw_sprite_scaled	(t_img *img, t_object *obj, unsigned x,
																unsigned src_x);

char			*atoi_limited	(unsigned *dst_int, const char *src_string,
															unsigned limit);
int				terminate		(t_game *game, int return_value, char *message);
void			terminate_free	(t_game *game);
void			write_screenshot_and_exit	(t_game *game);
void			write_screenshot_data	(t_game *game, int file_id);

#endif

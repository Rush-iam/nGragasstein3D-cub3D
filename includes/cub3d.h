/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:57:18 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/02 20:20:37 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "get_next_line.h"
# include "libft.h"
# include "mlx.h"
# include "x_events.h"

# ifndef __APPLE__
#  include "cute_png.h"
# endif

# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <string.h>
# include <time.h>

# define TITLE	"cub3D by nGragas"

# define ERR_MLX	1
# define ERR_ARGS	2
# define ERR_PARSE	3
# define ERR_MEM	4
# define ERR_BMP	5

# define WALL_N	0
# define WALL_S	1
# define WALL_W	2
# define WALL_E	3

# define K_EXIT			KEY_ESCAPE

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
# define PI2		6.28318530718

typedef struct s_upoint
{
	unsigned int	x;
	unsigned int	y;
}	t_upoint;

typedef struct s_fpoint
{
	double		x;
	double		y;
}	t_fpoint;

typedef struct s_img
{
	void		*ptr;
	unsigned	*data;
	t_upoint	size;
	double		aspect;
}	t_img;

typedef struct s_game
{
	bool			test;
	void			*mlx;
	void			*win;
	t_img			img;
	struct			s_player
	{
		t_fpoint	pos;
		double		angle;
		t_fpoint	vect;
	}				p;
	bool			key[512];
	struct			s_map
	{
		t_upoint	size;
		char		**grid;
	}				map;
	float			fov;
	float			col_center;
	double			col_step;
	double			col_scale;
	struct			s_column
	{
		unsigned int	height;
		double			texture_pos;
		char			dir;
	}				*column;
	unsigned int	color_ceil;
	unsigned int	color_floor;
	t_img			texture[4];
}	t_game;

void		player_set_fov(t_game *game, float fov, bool reset);
int			game_loop(t_game *game);

void		initialize_game(t_game *game, bool screenshot_only);

void		parse(int args, char **av, t_game *game, bool *screenshot_only);
void		parse_scene(int file_id, char **line, t_game *game);
void		parse_map(int file_id, char *line, t_game *game);
void		validate_settings(t_game *game);

void		set_ceilfloor(const char *color_string, unsigned int *target, \
							t_game *game);
void		set_textures(char *string, t_game *game);
void		set_textures_import(char *path, t_img *dst_img, t_game *game);

void		set_map(t_game *game, t_list *map);
void		set_map_process(t_game *game);
void		set_map_check_cell(t_game *game, char **map, t_upoint pt);

void		player_control(t_game *game);
int			hook_key_press(int key_code, t_game *game);
int			hook_key_release(int key_code, t_game *game);
int			hook_exit(t_game *game);

void		player_control_borders(t_game *g);

void		ray_cast(t_game *game);
void		ray_intersect(t_game *g, double angle, unsigned int ray);
t_fpoint	ray_intersect_x(t_game *game, t_fpoint step);
t_fpoint	ray_intersect_y(t_game *game, t_fpoint step);

void		img_ceilfloor_fill_rgb(t_img *img, int ceil, int floor);
void		draw_walls(t_game *g);
void		draw_wall_scaled(t_img *game_img, const t_img *src_img, \
							const struct s_column *column, unsigned int x);

char		*atoi_limited(unsigned int *dst_int, const char *src_string, \
														unsigned int limit);
int			terminate(t_game *game, int return_value, char *message);
void		free_resources(t_game *g);
void		write_screenshot_and_exit(t_game *game);
void		write_screenshot_data(t_game *game, int file_id);

#endif

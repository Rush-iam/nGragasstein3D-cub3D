/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:29:00 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/13 00:08:55 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_BONUS_H
# define CUB3D_BONUS_H

# include <time.h>
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

# define WIN_TITLE	"nGragasstein 3D"
# define MAX_WIN	(t_upoint){2559, 1394}
# define MAX_SCR	(t_upoint){20000, 20000}
# define MIN_RES_X	2

# define PI2	(2 * M_PI)
# define NANSECS_PER_SEC	1000000000

# define ERR_MLX	1
# define ERR_ARGS	2
# define ERR_PARSE	3
# define ERR_MEM	4
# define ERR_BMP	5

# define K_MOVE_FORWARD	KEY_W
# define K_MOVE_BACK	KEY_S
# define K_MOVE_LEFT	KEY_A
# define K_MOVE_RIGHT	KEY_D
# define K_TURN_LEFT	KEY_LEFT
# define K_TURN_RIGHT	KEY_RIGHT
# define K_RUN			KEY_SHIFT_LEFT

# define COLOR_WHITE	0xFFFFFF
# define COLOR_YELLOW	0xFFFF00
# define COLOR_GREEN	0x7AFF40
# define COLOR_ORANGE	0xFF7A40
# define COLOR_GREY		0x808080

# define K_KNIFE	KEY_1
# define K_PISTOL	KEY_2
# define K_RIFLE	KEY_3
# define M_SHOOT	MOUSE_LEFT

# define SHOT_FRAME_ID	2
# define ANIM_TICKS		5
# define ANIM_KNIFE		(char []){0, 1, 2, 3, 2, 1}
# define ANIM_PISTOL	(char []){0, 1, 2, 3, 1}
# define ANIM_RIFLE		(char []){0, 1, 2, 1, 3}

# define K_MAP_TOGGLE		KEY_M
# define MAP_COLOR_DECOR	COLOR_GREY
# define MAP_COLOR_ENEMY	COLOR_ORANGE
# define MAP_COLOR_PICKUP	COLOR_GREEN

# define K_FOV_WIDE		KEY_NUMMINUS
# define K_FOV_TELE		KEY_NUMPLUS
# define K_FOV_RESET	KEY_NUMASTERISK
# define FOV_ZOOMSPEED	1.03

# define PL_SPEED		0.08
# define PL_RADIUS		0.3
# define FLOAT_FIX		0.0000001
# define MOUSE_SPEED	2000.
# define MAP_SCALE		8

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
	unsigned	*data;
	t_upoint	size;
	double		aspect;
}				t_img;

# define CHAR_DECOR		"^*$:;,!@%#&|{}_<`"
# define CHAR_PICKUP	"+HhAaZzXx"
# define CHAR_ENEMY		"nswe"
# define CHAR_SOLID		"$:;,!@%#&|"
# define CHAR_OBJECTS	CHAR_DECOR CHAR_PICKUP CHAR_ENEMY
# define CHAR_ALLOWED	" .0123456789NSWE" CHAR_OBJECTS

# define VAL_HEALTH_XL	200
# define VAL_HEALTH_L	25
# define VAL_HEALTH_M	10
# define VAL_AMMO_M		8
# define VAL_AMMO_S		4
# define VAL_SCORE_XL	5000
# define VAL_SCORE_L	1000
# define VAL_SCORE_M	500
# define VAL_SCORE_S	100
# define VAL_SCORE_KILL	100
# define W_KNIFE_MASK	0b001
# define W_PISTOL_MASK	0b010
# define W_RIFLE_MASK	0b100

# define START_HEALTH	100
# define START_AMMO		8
# define START_WEAPONS	W_KNIFE_MASK | W_PISTOL_MASK

# define ENEMY_HEALTH	25

typedef struct	s_object
{
	t_img		*sprite;
	t_fpoint	pos;
	t_upoint	size;
	double		distance;
	double		distance_real;
	enum		e_objtype
	{
		T_DECOR = 0,
		T_HEALTH_XL,
		T_HEALTH_L,
		T_HEALTH_M,
		T_RIFLE,
		T_AMMO,
		T_BONUS_XL,
		T_BONUS_L,
		T_BONUS_M,
		T_BONUS_S,
		T_ENEMY,
		T_AMMO_ENEMY
	}			type;
	struct		s_enemy
	{
		bool	targeted;
		double	angle;
		short	health;
		short	frame;
		time_t	tick_nextframe;
		time_t	tick_state_start;
		time_t	tick_state_end;
		enum	e_objstate
		{
			S_WAIT = 0,
			S_ATTACK,
			S_PAIN,
			S_SEARCH,
			S_DEAD
		}		state;
	}			*e;
}				t_object;

typedef struct	s_game
{
	void		*mlx;
	void		*win;
	t_img		img;
	unsigned	tick;
	unsigned	tick_diff;
	struct		s_player
	{
		t_fpoint	pos;
		double		angle;
		t_fpoint	vector;
		short		health;
		short		ammo;
		short		score;
		short		weapons_mask;
		enum	e_weapon
		{
			W_KNIFE = 0,
			W_PISTOL,
			W_RIFLE
		}		weapon_cur;
		struct	s_weapon
		{
			unsigned char	animation[5];
			unsigned		frame;
			unsigned		frames;
			unsigned		tick;
			unsigned		ticks;
			bool			lock;
		}		weapon;
		bool		weapon_shot;
		t_img		weapon_img[3][4];
		t_upoint	weapon_pos;
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
		bool		show;
	}			map;
	float		fov;
	t_upoint	win_center;
	float		col_center;
	double		col_step;
	double		col_scale;
	struct		s_column
	{
		double		distance;
		unsigned	height;
		t_fpoint	cell;
		double		texture_pos;
		char		dir;
	}			**column;
	unsigned	color_ceil;
	unsigned	color_floor;
	t_img		texture[20];
	t_img		sprite[sizeof(CHAR_OBJECTS) - 4];
	t_list		*objects;
	t_list		*pickups;
	t_list		*enemies;
	struct		s_effect
	{
		unsigned	frames;
		unsigned	frame_cur;
		enum	e_effect
		{
			EF_FLASH = 0
		}		type;
		unsigned	color;
		float		max_power;
	}			effect;
	t_img		effect_img;
}				t_game;

void			initialize_game		(t_game *game, bool screenshot_only);
void			initialize_game_2	(t_game *game);
void			player_set_fov				(t_game *game, double fov, bool reset);

int				game_loop		(t_game *game);

void			parse(int args, char **av, t_game *game, bool *screenshot_only);
void			parse_scene		(int file_id, char **line, t_game *game);
void			parse_map(int file_id, char *line, t_list *map, t_game *game);
void			validate_settings(t_game *game);

void			set_resolution		(const char *res_string, t_upoint *res,
															t_game *game);
void			set_colors			(const char *color_string, unsigned *target,
															t_game *game);
void			set_weapons			(char *string, t_game *game);
void			set_textures		(char *string, t_game *game);
void			set_textures_import(char *string, t_img *texture, t_game *game);

void			set_map				(t_game *game, t_list *map);
void			set_map_process		(t_game *game);
void			set_map_object_add(t_game *game, char chr, unsigned type,
											t_upoint pt);
void			set_map_check_cell	(t_game *game, char **map, t_upoint pt);

int				hook_key_press		(int key_code, t_game *game);
int				hook_key_release	(int key_code, struct s_key *key);
int				hook_mouse_press	(int btn, int x, int y, struct s_key *key);
int				hook_mouse_release	(int btn, int x, int y, struct s_key *key);
int				hook_exit			(t_game *game);

void			player_control			(t_game *game);
void			player_control_rotate	(t_game *game);
void			player_control_move		(t_game *game);
void			player_control_weapon(t_game *game);
void			player_set_fov			(t_game *game, double fov, bool reset);
void			player_control_toggler	(t_game *game, int key_code);
void			player_control_extra	(t_game *game);
void			player_control_borders	(t_game *g);
void			player_control_borders_diag(t_game *g);
void			player_set_weapon(t_game *game, enum e_weapon weapon);

void			ray_cast		(t_game *game);
void			ray_intersect	(t_game *game, double cur_angle, unsigned ray);
t_fpoint		ray_intersect_x	(t_game *game, t_fpoint step);
t_fpoint		ray_intersect_y	(t_game *game, t_fpoint step);

void			img_clear				(t_img *img);
void			img_clear_rgb			(t_img *img, unsigned color);
void			img_ceilfloor_rgb	(t_img *img, unsigned ceil, unsigned floor);

void			pixel_put	(t_img *img, unsigned x, unsigned y, int color);
int				pixel_fade	(int color, float fade);
int				pixel_fade_contrast(int color, float fade);
void			draw_line	(t_img *img, t_point p1, t_point p2, int color);
void			draw_square	(t_img *img, t_point center, int size, int color);
void			draw_4pts	(t_img *img, t_point *pts, int color);

void			draw_map_init	(t_game *game);
void			draw_map		(t_game *game);
void			draw_map_player	(t_game *game);
void			draw_map_objects(t_game *game);

void			draw_walls		(t_game *game);
void			draw_wall_scaled(t_game *game, t_img *src, unsigned x,
						float fade);
void			draw_wall_solid	(t_game *game, unsigned x, float fade);

void			objects				(t_game *g);
void			pickups(t_game *g);
void			enemies(t_game *g);
void			object_add(t_game *game, t_list **dst, t_object *obj);
void			draw_objects		(t_game *g);
int				objects_sort		(t_object *obj1, t_object *obj2);
int				objects_find(t_object *obj1, t_object *obj2);
bool			object_pickup(t_game *game, enum e_objtype type);

void			object_pickup_add(t_game *game, enum e_objtype type);
void			draw_sprite			(t_game *game, t_object *obj, double angle);
void			draw_sprite_scaled	(t_img *img, t_object *obj, unsigned x,
																unsigned src_x);

void			weapon(t_game *game, struct s_weapon *weapon);
void			draw_weapon(t_game *game, struct s_weapon *weapon);
void			weapon_shoot(t_game *game);

void			draw_effect(t_game *game, struct s_effect *ef);
void			effect_flash(t_game *game, unsigned color, float power);
void			effect_fizzlefade				(t_img *img, unsigned color);

char			*atoi_limited	(unsigned *dst_int, const char *src_string,
															unsigned limit);
t_img			img_resize(void *mlx_ptr, t_img *src_img, t_upoint dstres);
t_img			img_faded_copy(void *mlx_ptr, t_img *img);
void			write_screenshot_and_exit	(t_game *game);
void			write_screenshot_data	(t_game *game, int file_id);

int				terminate		(t_game *game, int return_value, char *message);
void			terminate_free	(t_game *game);
void			terminate_free_object(void *object);
void			void_function(void *ptr);

void			demo_fillrate	(t_game *mlx, int step);
void			demo_radar		(t_game *mlx, int rays);
void			demo_cursor		(t_game *game, int color);

#endif

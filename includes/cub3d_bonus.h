/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:29:00 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/30 00:15:31 by ngragas          ###   ########.fr       */
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
# include "cute_sound.h"
# include "mlx.h"
# include "libft.h"
# include "get_next_line.h"
# include "x_events.h"

# define WIN_TITLE	"nGragasstein 3D"
# define MAX_SCREENSHOT	(t_point){20000, 20000}
# define MIN_RES	2
# define MAX_PLAYING_SOUNDS	24
# define MAX_MESSAGE_LEN 64
# define MAX_TEXTURES	19
# define END_LEVEL_MESSAGE "Well done! Thanks for playing :)"
# define MUSIC_BG_ID	0
# define MUSIC_END_ID	1

# define HUD_FACE_FILES "./resources/face/face_"
# define HUD_BAR_FILE "./resources/hud_bar.png"
# define HUD_DIGITS_FILE "./resources/hud_digits.png"
# define HUD_KNIFE_FILE "./resources/hud_knife.png"
# define HUD_PISTOL_FILE "./resources/hud_pistol.png"
# define HUD_RIFLE_FILE "./resources/hud_rifle.png"
# define HUD_DIGITS "0123456789 "
# define HUD_VALUES_OFFSET_Y 0.395f
# define HUD_SCORE_X	0.126f
# define HUD_FLOOR_X	0.045f
# define HUD_LIVES_X	0.347f
# define HUD_HEALTH_X	0.528f
# define HUD_AMMO_X		0.68f
# define HUD_WEAPON_X	0.818f
# define HUD_WEAPON_Y	0.095f
# define HUD_FACE_X		0.423f
# define HUD_FACE_Y		0.052f
# define HUD_FACE_TICK_TIMER	30
# define HUD_FACE_LEVELS	7
# define HUD_FACE_DIRS		3

# define T_FPT_NULL	(t_fpoint){0.0f, 0.0f}
# define M_PI_F		(float)M_PI
# define M_PI_2_F	(float)M_PI_2
# define M_PI_4_F	(float)M_PI_4
# define PI2_F		(2.0f * M_PI_F)
# define NANSECS_PER_SEC	1000000000
# define TICKS_PER_SEC		60

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
# define K_USE			KEY_E
# define K_JUMP			KEY_SPACE
# define K_CROUCH		KEY_CONTROL_LEFT

# define K_KNIFE	KEY_1
# define K_PISTOL	KEY_2
# define K_RIFLE	KEY_3
# define M_SHOOT	MOUSE_LEFT

# define K_MAP_TOGGLE	KEY_M
# define K_MOUSE_TOGGLE	KEY_BACKSPACE
# define K_FOV_WIDE		KEY_NUMMINUS
# define K_FOV_TELE		KEY_NUMPLUS
# define K_FOV_RESET	KEY_NUMASTERISK

# define COLOR_WHITE	0xFFFFFF
# define COLOR_GREEN	0x7AFF40
# define COLOR_GREEN_FF	0x113322
# define COLOR_CYAN_F	0x33AA99
# define COLOR_RED		0xFF0000
# define COLOR_ORANGE	0xFF7A40
# define COLOR_YELLOW	0xFFFF00
# define COLOR_YELLOW_F	0x888015
# define COLOR_GREY		0x808080

# define SHOT_FRAME_ID	2
# define ANIM_TICKS		5
# define ANIM_KNIFE		(char []){0, 1, 2, 3, 2, 1}
# define ANIM_PISTOL	(char []){0, 1, 2, 3, 1}
# define ANIM_RIFLE		(char []){0, 1, 2, 1, 3}

# define MAP_COLOR_DECOR	COLOR_GREY
# define MAP_COLOR_ENEMY	COLOR_ORANGE
# define MAP_COLOR_PICKUP	COLOR_GREEN

# define FOV_ZOOMSPEED	1.03f

# define PL_SPEED		0.07f
# define PL_RADIUS		0.4f
# define PL_JUMP		0.033f
# define PL_GRAVITY		-0.1f
# define PL_CROUCH_Z	-0.2f
# define PL_CROUCH_SPEED 5.0f
# define FLOAT_FIX		0.00001f
# define MOUSE_SPEED_X	2000.f
# define MAP_SCALE		(24 / 2)

typedef struct	s_point
{
	int	x;
	int	y;
}				t_point;

typedef struct	s_upoint
{
	unsigned	x;
	unsigned	y;
}				t_upoint;

typedef struct	s_fpoint
{
	float		x;
	float		y;
}				t_fpoint;

typedef struct	s_ray
{
	t_fpoint	pos;
	t_point		cell;
}				t_ray;

typedef struct	s_img
{
	void		*ptr;
	unsigned	*data;
	t_upoint	size;
	float		aspect;
	float		min_x;
	float		max_x;
	bool		*alpha_y;
}				t_img;

typedef struct	s_snd
{
	cs_loaded_sound_t	file;
	cs_play_sound_def_t	props;
}				t_snd;

# define CHAR_ELEVATOR	";"
# define CHAR_ELEVATOR_2 "<"
# define CHAR_WALLS		"0123456789:" CHAR_ELEVATOR CHAR_ELEVATOR_2
# define CHAR_DECOR		"^*$(),!@%#&?[]{}_~`"
# define CHAR_PICKUP	"+HhAaZzXx"
# define CHAR_ENEMY		"nswe"
# define CHAR_SOLID		"$(),!@%#&?[]"
# define CHAR_SOLID_MAP	'"'
# define CHAR_OBJECTS	CHAR_DECOR CHAR_PICKUP
# define CHAR_DOOR_1_H	"-"
# define CHAR_DOOR_1_V	"|"
# define CHAR_DOOR_1	CHAR_DOOR_1_H CHAR_DOOR_1_V
# define CHAR_DOOR_2_H	">"
# define CHAR_DOOR_2_V	"v"
# define CHAR_DOOR_2	CHAR_DOOR_2_H CHAR_DOOR_2_V
# define CHAR_DOORS_H	CHAR_DOOR_1_H CHAR_DOOR_2_H
# define CHAR_DOORS_V	CHAR_DOOR_1_V CHAR_DOOR_2_V
# define CHAR_DOORS		CHAR_DOOR_1 CHAR_DOOR_2 CHAR_ELEVATOR
# define CHAR_ALLOWED	" .NSWE" CHAR_WALLS CHAR_DOORS CHAR_OBJECTS CHAR_ENEMY
# define TEXTURE_DOOR_1		13
# define TEXTURE_DOOR_1_W	14
# define TEXTURE_DOOR_2		15
# define TEXTURE_DOOR_2_W	16
# define TEXTURE_CEIL		17
# define TEXTURE_FLOOR		18

enum	e_sound
{
	SND_PLAYER_NOACTION,
	SND_PLAYER_PAIN,
	SND_PLAYER_DEATH,
	SND_ELEVATOR,
	SND_DOOR_OPEN,
	SND_DOOR_CLOSE,
	SND_KNIFE,
	SND_PISTOL,
	SND_RIFLE,
	SND_PICKUP_AMMO,
	SND_PICKUP_RIFLE,
	SND_PICKUP_HEALTH_M,
	SND_PICKUP_HEALTH_L,
	SND_PICKUP_HEALTH_XL,
	SND_PICKUP_BONUS_S,
	SND_PICKUP_BONUS_M,
	SND_PICKUP_BONUS_L,
	SND_PICKUP_BONUS_XL,
	SND_ENEMY_ALARM,
	SND_ENEMY_ATTACK,
	SND_ENEMY_DEATH,
};

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

# define DMG_KNIFE_MIN	1
# define DMG_KNIFE_MAX	16
# define DMG_SHOT_MIN	11
# define DMG_SHOT_MAX	35

# define START_HEALTH	100
# define START_AMMO		0
# define START_WEAPONS	W_KNIFE_MASK | W_PISTOL_MASK

# define ENEMY_HEALTH		25
# define ENEMY_FOV_HALF		M_PI_2_F
# define ENEMY_MISS_MAX		60
# define ENEMY_DMG_MIN		3
# define ENEMY_DMG_MAX		30
//# define ENEMY_DMG_MIN		1
//# define ENEMY_DMG_MAX		1
# define ENEMY_SPEED		0.012f
# define ANIM_ENEMY_TICKS	10
# define ANIM_ENEMY_WALK_FRAMES	4
# define ANIM_ENEMY_SLOWMO	2

# define DOOR_ANIM_TICKS	60
# define DOOR_TIMER_TICKS	300

typedef struct	s_set
{
	t_img		wait[8];
	t_img		walk[8][4];
	t_img		attack[3];
	t_img		pain[2];
	t_img		death[5];
	t_snd		s_alarm;
	t_snd		s_attack;
	t_snd		s_death[10];
	unsigned	s_death_count;
}				t_set;

typedef struct	s_door
{
	t_point				cell;
	bool				opening;
	float				part_opened;
	time_t				ticks_to_close;
	cs_playing_sound_t	*sound;
	bool				end_level;
}				t_door;

typedef struct	s_object
{
	t_img		*sprite;
	t_fpoint	pos;
	float		distance;
	float		distance_real;
	float		atan_diff;
	float		angle_to_p;
	struct		s_render
	{
		t_point		size;
		int			start_0;
		t_point		start;
		t_point		end;
		t_fpoint	step;
	}			render;
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
		enum		e_enemytype
		{
			ENEMY_GUARD = 0
		}			type;
		bool		see;
		bool		alarmed;
		bool		shot;
		float		angle;
		float		p_to_angle;
		short		health;
		t_fpoint	location;
		float		location_angle;
		t_fpoint	target;
		t_list		*path;
		t_point		path_target;
		float		path_angle;
		t_img		*imgset;
		short		frame;
		short		frames;
		time_t		tick;
		time_t		ticks;
		enum		e_objstate
		{
			ST_WAIT = 0,
			ST_WALK,
			ST_ATTACK,
			ST_PAIN,
			ST_DEATH
		}			state;
	}			*e;
}				t_object;

typedef struct	s_game
{
	void		*mlx;
	void		*win;
	t_upoint	resolution;
	t_img		img;
	int			img_pixelcount;
	t_img		img_bg;
	unsigned	color_ceil;
	unsigned	color_floor;
	unsigned	fade_distance;
	unsigned	tick;
	unsigned	tick_diff;
	t_img		texture[MAX_TEXTURES * 2];
	t_img		sprite[sizeof(CHAR_OBJECTS) - 1];
	t_set		enemyset[1];
	t_list		*doors;
	t_list		*objects;
	struct		s_map
	{
		t_img		img;
		t_upoint	size;
		char		**grid;
		unsigned	**grid_bfs;
		bool		enabled;
	}			map;
	t_point		center;
	int			horizon;
	float		z_level;
	float		z_level_vy;
	float		z_level_target;
	float		fov;
	float		*angles;
	float		col_step;
	float		col_scale;
	struct		s_column
	{
		float		distance;
		t_fpoint	pos;
		t_point		cell;
		char		dir;
		int			height;
		unsigned	texture_id;
		float		texture_pos;
	}			*column;
	struct		s_key
	{
		bool		k[280];
		bool		m[10];
		t_point		mpos;
		t_point		mdir;
		bool		mouse;
	}			key;
	struct		s_hud
	{
		bool		needs_redraw;
		float		scale;
		t_img		bar;
		t_img		digits;
		t_img		weapon[3];
		t_img		face[HUD_FACE_LEVELS][HUD_FACE_DIRS];
		t_img		face_dead;
		t_point		face_pos;
		unsigned	face_nexttick;
		float		digit_width;
	}			hud;
	struct		s_effect
	{
		unsigned	frame_cur;
		unsigned	frames;
		enum		e_effect
		{
			EF_FLASH = 0,
			EF_FIZZLEFADE
		}			type;
		unsigned	color;
		float		max_power;
	}			effect;
	struct		s_string
	{
		char		text[MAX_MESSAGE_LEN + 1];
		t_upoint	pos;
		unsigned	frame_cur;
		unsigned	frames;
		bool		fade;
		unsigned	color;
	}			string;
	t_img		img_effect;
	struct		s_sound
	{
		cs_context_t*	ctx;
		cs_context_t*	ctx7;
		cs_context_t*	ctx22;
		t_snd			music[2];
		t_snd			sound[21];
		struct			s_playing_sound
		{
			cs_playing_sound_t	*snd;
			t_fpoint			sourcepos;
		}				playing[MAX_PLAYING_SOUNDS];
	}			audio;
	struct		s_player
	{
		t_fpoint	pos;
		float		angle;
		t_fpoint	vect;
		short		health;
		short		ammo;
		int			score;
		t_object	*target;
		short		weapons_mask;
		enum		e_weapon
		{
			W_KNIFE = 0,
			W_PISTOL,
			W_RIFLE
		}			weapon_cur;
		struct		s_weapon
		{
			unsigned char	animation[5];
			unsigned short	frame;
			unsigned short	frames;
			unsigned short	tick;
			unsigned short	ticks;
			bool			lock;
		}			weapon;
		bool		weapon_shot;
		bool		weapon_noise;
		t_img		weapon_img[3][4];
		t_upoint	weapon_pos;
	}			p;
}				t_game;

// main
int				game_loop(t_game *game);
void			game_ticks(t_game *game);
int				dead_exit(t_game *game);
void			draw_ceilfloor(t_game *g);
bool			chr_is_wall(char c);

// parse
void			parse(int args, char **av, t_game *game, bool *screenshot_only);
void			parse_scene		(int file_id, char **line, t_game *game);
void			parse_map(int file_id, char *line, t_list *map, t_game *game);
void			validate_settings(t_game *game);

// parse_set
void			set_resolution		(const char *res_string, t_upoint *res,
									   t_game *game);
void			set_colors			(const char *color_string, unsigned *target,
									   t_game *game);
void			set_weapons			(char *string, t_game *game);
void			set_textures		(char *string, t_game *game);

// parse_set_map
void			set_map				(t_game *game, t_list *map);
void			set_map_process		(t_game *game);
void			set_map_door_add(t_game *game, t_upoint pt);
void			set_map_object_add(t_game *game, char chr, unsigned type,
								   t_upoint pt);
void			set_map_check_cell	(t_game *game, char **map, t_upoint pt);

// initialize
void			initialize_game(t_game *game, bool screenshot_only);
void			initialize_window(t_game *g, bool screenshot_only);
void			initialize_canvas_images(t_game *g);
void			initialize_hud_images(t_game *g);
void			initialize_face_images(t_game *g);
void			initialize_weapons_and_faded_walls(t_game *g);

// hooks
int				hook_key_press		(int key_code, t_game *game);
int				hook_key_release	(int key_code, struct s_key *key);
int				hook_mouse_press	(int btn, int x, int y, struct s_key *key);
int				hook_mouse_release	(int btn, int x, int y, struct s_key *key);
int				hook_exit			(t_game *game);

// control
void			player_control			(t_game *game);
void			player_control_rotate	(t_game *game);
void			player_control_move		(t_game *game);
void			player_control_jump_n_crouch(t_game *g);
void			player_control_weapon(t_game *game);
void			player_set_fov			(t_game *game, float fov, bool reset);
void			player_control_toggler	(t_game *g, int key_code);
void			player_control_extra	(t_game *game);
void			player_control_borders	(t_game *g);
void			player_control_borders_diag(t_game *g);
void			player_set_weapon(t_game *game, enum e_weapon weapon);

// sounds
void			initialize_sounds(t_game *g);
void			sounds(t_game *game);
cs_playing_sound_t *	sound_play(t_game *game, t_snd *sound, t_fpoint sourcepos);
void			music_play(t_game *game, t_snd *music);
void			sound_adjust_pan(struct s_player *pl, struct s_playing_sound sound);

// doors
void			doors(t_game *game);
t_door			*door_find(t_game *game, t_point cell);
void			door_open(t_game *g, t_point cell, bool by_player);
void			door_sound(t_game *game, t_door *door, t_snd *sound);

// weapons
void			weapon(t_game *game, struct s_weapon *weapon);
void			draw_weapon(t_game *game, struct s_weapon *weapon);
void			weapon_shoot(t_game *g, t_object *target);

// objects
void			objects				(t_game *g);
void			object_add(t_game *game, t_list **dst, void *obj);
int				objects_sort		(t_object *obj1, t_object *obj2);

// pickup
bool			pickup(t_game *game, enum e_objtype type);
void			pickup_drop(t_game *game, t_fpoint pos, enum e_objtype type, t_img *img);
void			pickup_get(t_game *game, enum e_objtype item);
void			pickup_sound(t_game *game, enum e_objtype item);

// enemies
void			enemy(t_game *game, t_object *obj);
void			enemy_logic(t_game *game, t_object *obj);
void			enemy_sprite(t_game *game, t_object *obj);
void			enemy_shoot(t_game *g, t_object *obj);
void			enemy_set_state(t_game *g, t_object *obj, enum e_objstate state);
void			enemy_sound(t_game *game, t_object *obj, enum e_sound sound_type);

// pathfind
void			initialize_bfs_grid(t_game *g);
void			*pathfind(t_list **path, t_point from, t_point to, struct s_map *map);
bool			pathfind_init(struct s_map *map, t_point target, t_list **queue, t_point **pt);
void			*pathfind_deinit(t_list **queue, t_point *pt);
int				pathfind_nears(struct s_map *map, t_point pt, t_point *nears);
void			*pathfind_construct(t_list **path, t_point from, struct s_map *map);

// raycasting
void			ray_cast		(t_game *game);
struct s_column	ray_intersect	(t_game *g, float tan_angle, t_point negative);
float			ray_intersect_distance(t_game *game, float angle);
t_ray			ray_intersect_x	(t_game *g, int step_x, float step_y);
t_ray			ray_intersect_y(t_game *g, float step_x, int step_y);

// draw_walls
void			draw_wall_texture_set(t_game *g, struct s_column *col, t_point pt);
void			draw_door_texture_set(t_game *game, struct s_column *col, char chr);
void			draw_walls		(t_game *g);
void			draw_wall_scaled(t_game *g, t_img src, t_point cur, int z_offset);
void			draw_wall_scaled_f(t_game *g, t_img src, t_point cur, int z_offset);
void			draw_wall_solid	(t_game *game, unsigned x, float fade);

// draw_objects
void			draw_object_properties(t_game *game, t_object *obj);
void			draw_objects(t_game *game);
void			draw_sprite(t_game *game, t_object *obj);
void			draw_sprite_scaled(t_game *g, t_object *obj, t_point min, t_point max);
void			draw_sprite_scaled_f(t_game *g, t_object *obj, t_point min, t_point max);

// draw_hud
void			draw_hud(t_game *g);
void			draw_hud_face(t_game *g, bool force_redraw);
char			*hud_num_to_str(char num_str[11], unsigned num, int length);
void			hud_digits_bake(struct s_hud *hud, char num_str[], t_upoint pos);
void			draw_fps(t_game *game);

// draw_map
void			initialize_map_hud	(t_game *game);
void			draw_map		(t_game *game);
void			draw_map_player	(t_game *g);
void			draw_map_objects(t_game *game);

// load_files
void			load_image_file(t_game *g, char *path, t_img *dst_img, char *err);
void			load_audio_file(t_snd *dst, char *path);
void			load_audioset(t_set *dst, char *path, t_game *game);
void			load_spriteset(t_img dst[], int count, char *path, t_game *game);

// image_utils
t_img			*img_create(void *mlx_ptr, t_img *dst, t_upoint size);
t_img			*img_resize(void *mlx_ptr, t_img *src_img, t_upoint dstres);
void			img_from_file_scaled(t_game *g, char *path, t_img *dst_img, float scale);
void			img_alpha_columns_get(t_img *img);
t_img			img_faded_copy(void *mlx_ptr, t_img *img);

// draw_pixels
void			pixel_put	(t_img *img, unsigned x, unsigned y, int color);
int				pixel_fade	(int color, float fade);
int				pixel_alpha	(int color, float alpha);
int				pixel_fade_contrast(int color, float fade);

// draw_fills
void			img_clear				(t_img *img);
void			img_clear_rgb			(t_img *img, unsigned color);
void			img_ceilfloor_rgb	(t_img *img, unsigned ceil, unsigned floor);
void			img_ceilfloor_rgb_faded(t_img *img, unsigned ceil, unsigned floor, unsigned fade_distance);

// draw_figures
void			draw_line	(t_img *img, t_point p1, t_point p2, int color);
void			draw_square	(t_img *img, t_point center, int size, int color);
void			draw_square_fill(t_img *img, t_point top_left, int size, int color);
void			draw_4pts	(t_img *img, t_point *pts, int color);

// effects
void			draw_effect(t_game *game, struct s_effect *ef);
void			effect_flash(t_game *game, unsigned color, float power);
void			effect_fizzlefade(t_game *game, unsigned color);

// utils
char			*atoi_limited(unsigned *dst_int, const char *src_string,
							  unsigned limit);
void			string_add(t_game *g, char *string, int timer, unsigned color);
void			draw_string(t_game *g, struct s_string *s);

// screenshot
void			write_screenshot_and_exit	(t_game *game);
void			write_screenshot_data	(t_game *game, int file_id);

// terminate
int				terminate		(t_game *game, int return_value, char *message);
void			terminate_free	(t_game *game);
void			terminate_free_images(t_game *game, t_img *arr, unsigned count);
void			terminate_free_object(void *object);
void			terminate_audio(t_game *game);

// demo_tools
void			demo_fillrate	(t_game *mlx, int step);
void			demo_radar		(t_game *mlx, int rays);
void			demo_cursor		(t_game *game, int color);

#endif

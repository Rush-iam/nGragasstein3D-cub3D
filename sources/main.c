/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:58:52 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/10 22:49:59 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "x_events.h"

int	main(void)
{
	t_game	game;
	int		null;

	game = (t_game){0};
	if ((game.mlx = mlx_init()) == NULL)
		return (EXIT_FAILURE);
	if (!(game.win = mlx_new_window(game.mlx, WIN_W, WIN_H, "cub3D by nGragas")))
		return (EXIT_FAILURE);
	if (!(game.img.ptr = mlx_new_image(game.mlx, WIN_W, WIN_H)))
		return (EXIT_FAILURE);
	game.img.data = (int *)mlx_get_data_addr(game.img.ptr, &null, &null, &null);
	game.img.size = (t_upoint){WIN_W, WIN_H};
	game.map.size = (t_upoint){25, 14};
	game.map.img.size = (t_upoint){game.map.size.x * MAP_SCALE,
								game.map.size.y * MAP_SCALE};
	if (!(game.map.img.ptr = mlx_new_image(game.mlx,
							game.map.img.size.x, game.map.img.size.y)))
		return (EXIT_FAILURE);
	game.map.img.data = (int *)mlx_get_data_addr(game.map.img.ptr, &null, &null,
																		&null);
	mlx_do_key_autorepeatoff(game.mlx);
	mlx_hook(game.win, EVENT_KEYPRESS, 0, hook_key_press, &game.key);
	mlx_hook(game.win, EVENT_KEYRELEASE, 0, hook_key_release, &game.key);
	mlx_hook(game.win, EVENT_BUTTONPRESS, 0, hook_mouse_press, &game.key);
	mlx_hook(game.win, EVENT_BUTTONRELEASE, 0, hook_mouse_release, &game.key);
	mlx_hook(game.win, EVENT_MOTIONNOTIFY, 0, hook_mouse_move, &game.key);
	mlx_hook(game.win, EVENT_DESTROYNOTIFY, 0, terminate, NULL);
	mlx_loop_hook(game.mlx, game_loop, &game);

	char map[][25] = {	"1111111111111111111111111",
						"1000000000110000000000001",
						"1011000001110000001000001",
						"1001000000000000000000001",
						"1111111110110000011100001",
						"1000000000110000011101111",
						"1111011111111101110000001",
						"1111011111111101110101001",
						"1100000011010101110000001",
						"1000100000000000000000001",
						"1000000000000000110101001",
						"1100000111010101111101111",
						"1111011111111111111111111",
						"1111111111111111111111111"};
	int i = 0;
	while (i < 14)
	{
		game.map.grid[i] = map[i];
		i++;
	}
	game.map.size = (t_upoint){25, 14};
	game.p.pos = (t_fpoint){12.1, 3.5};

	if (!(game.wall[WALL_N].ptr = mlx_xpm_file_to_image(game.mlx, "resources/WALL_N.xpm",
					(int *)&game.wall[WALL_N].size.x, (int *)&game.wall[WALL_N].size.y)))
		return (EXIT_FAILURE);
	game.wall[WALL_N].data = (int *)mlx_get_data_addr(game.wall[WALL_N].ptr, &null, &null, &null);
	if (!(game.wall[WALL_S].ptr = mlx_xpm_file_to_image(game.mlx, "resources/WALL_S.xpm",
					(int *)&game.wall[WALL_S].size.x, (int *)&game.wall[WALL_S].size.y)))
		return (EXIT_FAILURE);
	game.wall[WALL_S].data = (int *)mlx_get_data_addr(game.wall[WALL_S].ptr, &null, &null, &null);
	if (!(game.wall[WALL_W].ptr = mlx_xpm_file_to_image(game.mlx, "resources/WALL_W.xpm",
					(int *)&game.wall[WALL_W].size.x, (int *)&game.wall[WALL_W].size.y)))
		return (EXIT_FAILURE);
	game.wall[WALL_W].data = (int *)mlx_get_data_addr(game.wall[WALL_W].ptr, &null, &null, &null);
	if (!(game.wall[WALL_E].ptr = mlx_xpm_file_to_image(game.mlx, "resources/WALL_E.xpm",
					(int *)&game.wall[WALL_E].size.x, (int *)&game.wall[WALL_E].size.y)))
		return (EXIT_FAILURE);
	game.wall[WALL_E].data = (int *)mlx_get_data_addr(game.wall[WALL_E].ptr, &null, &null, &null);
	if (!(game.sprite[0].img.ptr = mlx_xpm_file_to_image(game.mlx, "resources/sprite.xpm",
					(int *)&game.sprite[0].img.size.x, (int *)&game.sprite[0].img.size.y)))
		return (EXIT_FAILURE);
	game.sprite[0].img.data = (int *)mlx_get_data_addr(game.sprite[0].img.ptr, &null, &null, &null);
	game.sprite[0].frames = 1;
	game.object[0].pos = (t_fpoint){20.5, 3.5};
	game.object[0].sprite = &game.sprite[0];
	game.object[1].pos = (t_fpoint){21.5, 3.5};
	game.object[1].sprite = &game.sprite[0];
	game.object[2].pos = (t_fpoint){22, 3.2};
	game.object[2].sprite = &game.sprite[0];
	game.object[3].pos = (t_fpoint){22, 3};
	game.object[3].sprite = &game.sprite[0];
	game.object_count = 4;
	mlx_loop(game.mlx);
}

void	player_control(t_game *game)
{
	if (game->key.k[TURN_LEFT])
		game->p.angle -= 0.02;
	if (game->key.k[TURN_RIGHT])
		game->p.angle += 0.02;
	if (game->p.angle > PI2)
		game->p.angle -= PI2;
	if (game->p.angle < 0)
		game->p.angle += PI2;
	if (game->key.k[MOVE_FORWARD])
	{
		game->p.pos.x += PLAYER_SPEED * cos(game->p.angle);
		game->p.pos.y += PLAYER_SPEED * sin(game->p.angle);
	}
	if (game->key.k[MOVE_BACK])
	{
		game->p.pos.x -= PLAYER_SPEED * cos(game->p.angle);
		game->p.pos.y -= PLAYER_SPEED * sin(game->p.angle);
	}
	if (game->key.k[MOVE_LEFT])
	{
		game->p.pos.x += PLAYER_SPEED * sin(game->p.angle);
		game->p.pos.y -= PLAYER_SPEED * cos(game->p.angle);
	}
	if (game->key.k[MOVE_RIGHT])
	{
		game->p.pos.x -= PLAYER_SPEED * sin(game->p.angle);
		game->p.pos.y += PLAYER_SPEED * cos(game->p.angle);
	}
	if (game->p.pos.x < 0)
		game->p.pos.x = 0;
	if (game->p.pos.y < 0)
		game->p.pos.y = 0;
	if (game->p.pos.x >= game->map.size.x)
		game->p.pos.x = nextafter(game->map.size.x, game->map.size.x - 1);
	if (game->p.pos.y >= game->map.size.y)
		game->p.pos.y = nextafter(game->map.size.y, game->map.size.y - 1);
}

void	ray_intersect(t_game *game, double cur_angle, unsigned ray)
{
	t_fpoint	x_axis;
	t_fpoint	y_axis;
	t_fpoint	len;
	t_fpoint	step;

	step = (cur_angle < M_PI_2 || cur_angle > 3 * M_PI_2) ?
			(t_fpoint){1, tan(cur_angle)} : (t_fpoint){-1, -tan(cur_angle)};
	x_axis.x = floor(game->p.pos.x + (step.x > 0));
	x_axis.y = game->p.pos.y + step.y *
			(step.x > 0 ? 1 - (game->p.pos.x - (int)game->p.pos.x) :
								game->p.pos.x - (int)game->p.pos.x);
	while ((unsigned)x_axis.y < game->map.size.y &&
			(unsigned)x_axis.x - (step.x < 0) < game->map.size.x &&
			game->map.grid[(int)x_axis.y][(int)x_axis.x - (step.x < 0)] != '1')
		x_axis = (t_fpoint){round(x_axis.x + step.x), x_axis.y + step.y};
	if (x_axis.y > game->map.size.y)
		x_axis.y = game->map.size.y;
	step = (cur_angle < M_PI) ?
			(t_fpoint){tan(M_PI_2 - cur_angle), 1} : (t_fpoint){-tan(M_PI_2 - cur_angle), -1};
	y_axis.y = floor(game->p.pos.y + (step.y > 0));
	y_axis.x = game->p.pos.x + step.x *
			(step.y > 0 ? 1 - (game->p.pos.y - (int)game->p.pos.y) :
								game->p.pos.y - (int)game->p.pos.y);
	while ((unsigned)y_axis.y - (step.y < 0) < game->map.size.y &&
			(unsigned)y_axis.x < game->map.size.x &&
			game->map.grid[(int)y_axis.y - (step.y < 0)][(int)y_axis.x] != '1')
		y_axis = (t_fpoint){y_axis.x + step.x, round(y_axis.y + step.y)};
	if (y_axis.x > game->map.size.x)
		y_axis.x = game->map.size.x;
	len.x = sqrt(pow(x_axis.x - game->p.pos.x, 2) + pow(x_axis.y - game->p.pos.y, 2));
//	len.x = fabs((x_axis.y - game->p.pos.y) / sin(cur_angle));
	len.y = sqrt(pow(y_axis.x - game->p.pos.x, 2) + pow(y_axis.y - game->p.pos.y, 2));
//	len.y = fabs((y_axis.x - game->p.pos.x) / cos(cur_angle));
	game->column[ray].distance = (len.x < len.y ? len.x : len.y) *
												cos(cur_angle - game->p.angle);
	game->column[ray].height = COL_SCALE / game->column[ray].distance;
	if (game->column[ray].height == 0 || game->column[ray].height > 8 * COL_SCALE)
		game->column[ray].height = 8 * COL_SCALE;
	game->column[ray].cell = (len.x < len.y) ? x_axis : y_axis;
	game->column[ray].dir = (len.x < len.y) ?
			"EW"[x_axis.x >= game->p.pos.x] : "NS"[y_axis.y >= game->p.pos.y];
	game->column[ray].texture_pos = (len.x < len.y) ?
		fabs(x_axis.y) - (int)x_axis.y : fabs(y_axis.x) - (int)y_axis.x;
}

void	draw_map_player(t_game *game)
{
	unsigned	ray;

	draw_square(&game->map.img, (t_point){game->p.pos.x * MAP_SCALE,
									game->p.pos.y * MAP_SCALE}, 6, 0xFFFA80);
	ray = 0;
	while (ray < game->img.size.x)
	{
		draw_line(&game->map.img,
			(t_point){game->p.pos.x * MAP_SCALE, game->p.pos.y * MAP_SCALE},
			(t_point){game->column[ray].cell.x * MAP_SCALE,
					game->column[ray].cell.y * MAP_SCALE}, 0x888015);
		ray += 32;
	}
	draw_line(&game->map.img, (t_point){game->p.pos.x * MAP_SCALE,
									game->p.pos.y * MAP_SCALE}, (t_point){
		game->p.pos.x * MAP_SCALE + MAP_SCALE * cos(game->p.angle),
		game->p.pos.y * MAP_SCALE + MAP_SCALE * sin(game->p.angle)}, 0xFF4020);
}

void	draw_map(t_game *game)
{
	unsigned	x;
	unsigned	y;

	img_clear_rgb(&game->map.img, 0xAA000000);
	y = 0;
	while (y < game->map.img.size.y)
	{
		x = 0;
		while (x < game->map.img.size.x)
		{
			draw_line(&game->map.img, (t_point){x, y},
					  (t_point){x + MAP_SCALE, y}, 0x113322);
			draw_line(&game->map.img, (t_point){x, y},
					  (t_point){x, y + MAP_SCALE}, 0x113322);
			if (game->map.grid[y / MAP_SCALE][x / MAP_SCALE] == '1')
				draw_square(&game->map.img, (t_point){
				x + MAP_SCALE / 2, y + MAP_SCALE / 2}, MAP_SCALE - 4, 0x33AA99);
			x += MAP_SCALE;
		}
		y += MAP_SCALE;
	}
	draw_map_player(game);
	draw_square(&game->map.img, (t_point){game->object[0].pos.x * MAP_SCALE,
							game->object[0].pos.y * MAP_SCALE}, 6, 0xFFFA80);
	mlx_put_image_to_window(game->mlx, game->win, game->map.img.ptr, 0, 0);
}

int		pixel_fade(int pixel, double fade)
{
	return (((int)((pixel & 0xFF0000) * fade) & 0xFF0000) +
	((int)((pixel & 0xFF00) * fade) & 0xFF00) + (int)((pixel & 0xFF) * fade));
}

void	draw_wall_scaled(t_game *game, t_img *src, unsigned ray, double fade)
{
	const double	step = (double)src->size.y / game->column[ray].height;
	const unsigned	src_column = game->column[ray].texture_pos * src->size.x;
	unsigned		cur;
	double			cur_src;
	unsigned		max_height;

	cur_src = 0;
	if (game->column[ray].height > game->img.size.y)
	{
		cur_src += (double)src->size.y * (game->column[ray].height -
							game->img.size.y) / 2 / game->column[ray].height;
		cur = 0;
		max_height = game->img.size.y;
	}
	else
	{
		cur = game->img.size.y / 2 - game->column[ray].height / 2;
		max_height = cur + game->column[ray].height;
	}
	while (cur < max_height)
	{
		if (fade == 1)
			pixel_put(&game->img, ray, cur,
				src->data[(unsigned)cur_src * src->size.x + src_column]);
		else
			pixel_put(&game->img, ray, cur, pixel_fade(
				src->data[(unsigned)cur_src * src->size.x + src_column], fade));
		cur_src += step;
		cur++;
	}
}

void	draw_walls(t_game *game)
{
	unsigned	ray;
	double		fade;
	unsigned	texture_id;
	const char	*textures = "NSWE";

	ray = 0;
	while (ray < game->img.size.x)
	{
		fade = 4. / game->column[ray].distance;
		if (fade > 1)
			fade = 1;
		texture_id = ft_strchr(textures, game->column[ray].dir) - textures;
		draw_wall_scaled(game, &game->wall[texture_id], ray, fade);
//		draw_line(&game->img,
//			(t_point){ray, game->img.size.y / 2 - game->column[ray].height / 2},
//			(t_point){ray, game->img.size.y / 2 + game->column[ray].height / 2},
//		((int)(0x60 * fade) << 16) + ((int)(0x80 * fade) << 8) + (int)(0x80 * fade));
		ray++;
	}
}

void	get_wall_intersections(t_game *game)
{
	unsigned		ray;
	const double	start_angle = game->p.angle - FOV / 2;
	double			cur_angle;

	ray = 0;
	while (ray < game->img.size.x)
	{
		cur_angle = start_angle + FOV * ray / (game->img.size.x - 1);
		if (cur_angle < 0)
			cur_angle += PI2;
		else if (cur_angle > PI2)
			cur_angle -= PI2;
		ray_intersect(game, cur_angle, ray);
		ray++;
	}
}

void	draw_sprite_scaled(t_game *game, t_object *obj, int ray, unsigned src_col)
{
	const double	step = (double)obj->sprite->img.size.y / obj->height;
	unsigned		cur;
	double			cur_src;
	unsigned		max_height;
	int				src_pixel;

	cur_src = 0;
	if (obj->height > game->img.size.y)
	{
		cur_src += (double)obj->sprite->img.size.y * (obj->height -
					game->img.size.y) / 2 / obj->height;
		cur = 0;
		max_height = game->img.size.y;
	}
	else
	{
		cur = game->img.size.y / 2 - obj->height / 2;
		max_height = cur + obj->height;
	}
	while (cur < max_height)
	{
		src_pixel = obj->sprite->img.data[(unsigned)cur_src * obj->sprite->img.size.x + src_col];
		if ((src_pixel >> 24) == 0)
			pixel_put(&game->img, ray, cur, src_pixel);
		cur_src += step;
		cur++;
	}
}

void	sprites_sort(t_game *game)
{
	unsigned	i;
	bool		sorted;
	t_object	tmp;

	if (game->object_count == 0)
		return ;
	sorted = false;
	while (sorted == false)
	{
		sorted = true;
		i = 0;
		while (i < game->object_count - 1)
		{
			if (game->object[i].distance < game->object[i + 1].distance)
			{
				sorted = false;
				tmp = game->object[i];
				game->object[i] = game->object[i + 1];
				game->object[i + 1] = tmp;
			}
			i++;
		}
	}
}

void	draw_sprites(t_game *game)
{
	unsigned int	i;
	double			angle;
	t_fpoint		diff;
	int				cur_ray;
	int				max_ray;

	sprites_sort(game);
	i = 0;
	while (i < game->object_count)
	{
		diff = (t_fpoint){game->object[i].pos.x - game->p.pos.x,
						game->object[i].pos.y - game->p.pos.y};
		angle = atan2(diff.y, diff.x);
		if (fabs(game->p.angle - angle - PI2) <= FOV / 2)
			angle += PI2;
		angle = game->p.angle - angle;
		if (fabs(angle) <= FOV / 2)
		{
			game->object[i].distance = sqrt(pow(diff.x, 2) + pow(diff.y, 2));
			game->object[i].height = COL_SCALE / game->object[i].distance;
			cur_ray = (1. - (angle + FOV / 2) / FOV) * game->img.size.x;
			cur_ray -= game->object[i].height / 2;
			max_ray = cur_ray + game->object[i].height;
			while (cur_ray < max_ray && cur_ray < (int)game->img.size.x)
			{
				if (cur_ray >= 0 && game->column[cur_ray].distance > game->object[i].distance)
					draw_sprite_scaled(game, &game->object[i], cur_ray,
				(1. - (double)(max_ray - cur_ray) / game->object[i].height) *
						game->object[i].sprite->img.size.x);
				cur_ray++;
			}
		}
		i++;
	}
}

int	game_loop(t_game *game)
{
	static clock_t	tick;
	char			*fps;

	player_control(game);
//	img_clear(&game->img);
//	img_clear_rgb(&game->img, 0x051400);
//	img_ceilfloor_fill(&game->img, COLOR_CEIL, COLOR_FLOOR);

	img_ceilfloor_fill_rgb(&game->img, COLOR_CEIL, COLOR_FLOOR);
	get_wall_intersections(game);
	draw_walls(game);
	draw_sprites(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.ptr, 0, 0);
	draw_map(game);

//	fizzlefade(&game->img, 0xFF0000);
//	demo_fillrate(game, 1);
//	demo_radar(game, 360);
//	demo_cursor(game, 0xFF88FF);
//	ft_putnbr_fd(CLOCKS_PER_SEC / (clock() - start), 1);
//	write(1, "\n", 1);
	fps = ft_itoa(CLOCKS_PER_SEC / (clock() - tick));
	tick = clock();
	mlx_string_put(game->mlx, game->win, 0, 0, 0xFFFFFF, fps);
	free(fps);

//	write(1, "Player x", 9); ft_putnbr_fd((int)game->p.pos.x, 1);
//	write(1, " y", 3); ft_putnbr_fd((int)game->p.pos.y, 1); write(1, "\n", 1);
//	write(1, "Column 0 height ", 16); ft_putnbr_fd(game->column[0].height, 1);
//	write(1, " cell x", 7); ft_putnbr_fd((int)game->column[0].cell.x, 1);
//	write(1, " y", 2); ft_putnbr_fd((int)game->column[0].cell.y, 1);
//	write(1, " dir=", 5); write(1, &(game->column[0].dir), 1); write(1, "\n", 1);
	return (0);
}

int	terminate(int status)
{
	exit(status);
}
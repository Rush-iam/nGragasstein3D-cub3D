/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:58:52 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/07 20:39:15 by ngragas          ###   ########.fr       */
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
	game.map.size = (t_upoint){WIN_W, WIN_H};
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
	game.p.pos = (t_fpoint){25. / 2, 14 / 2};
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

void	draw_map(t_game *game)
{
	const unsigned	max_x = game->map.size.x * MAP_SCALE;
	const unsigned	max_y = game->map.size.y * MAP_SCALE;
	unsigned int	x;
	unsigned int	y;

	y = 0;
	while (y < max_y)
	{
		x = 0;
		while (x < max_x)
		{
			draw_line(&game->img, (t_point){x, y},
									(t_point){x + MAP_SCALE, y}, 0x113322);
			draw_line(&game->img, (t_point){x, y},
									(t_point){x, y + MAP_SCALE}, 0x113322);
			if (game->map.grid[y / MAP_SCALE][x / MAP_SCALE] == '1')
				draw_square(&game->img, (t_point){
				x + MAP_SCALE / 2, y + MAP_SCALE / 2}, MAP_SCALE - 4, 0x33AA99);
			x += MAP_SCALE;
		}
		y += MAP_SCALE;
	}
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
	len.y = sqrt(pow(y_axis.x - game->p.pos.x, 2) + pow(y_axis.y - game->p.pos.y, 2));
	game->column[ray].distance = (len.x < len.y) ? len.x : len.y;
	game->column[ray].height = COL_SCALE * 1. / sqrt(game->column[ray].distance);
	if (game->column[ray].height == 0 || game->column[ray].height > 8 * COL_SCALE)
		game->column[ray].height = 8 * COL_SCALE;
	game->column[ray].cell = (len.x < len.y) ? x_axis : y_axis;
	game->column[ray].dir = (len.x < len.y) ?
			"EW"[x_axis.x >= game->p.pos.x] : "NS"[y_axis.y >= game->p.pos.y];
}

void	draw_player(t_game *game)
{
	unsigned	ray;

	draw_square(&game->img, (t_point){game->p.pos.x * MAP_SCALE,
									game->p.pos.y * MAP_SCALE}, 6, 0xFFFA80);
	ray = 0;
	while (ray < game->img.size.x)
	{
		draw_line(&game->img,
			(t_point){game->p.pos.x * MAP_SCALE, game->p.pos.y * MAP_SCALE},
			(t_point){game->column[ray].cell.x * MAP_SCALE,
					game->column[ray].cell.y * MAP_SCALE}, 0x888015);
		ray++;
	}
	draw_line(&game->img, (t_point){game->p.pos.x * MAP_SCALE,
									game->p.pos.y * MAP_SCALE}, (t_point){
		game->p.pos.x * MAP_SCALE + MAP_SCALE * cos(game->p.angle),
		game->p.pos.y * MAP_SCALE + MAP_SCALE * sin(game->p.angle)}, 0xFF4020);
}

void	draw_walls(t_game *game)
{
	const unsigned	center = game->img.size.y / 2;
	unsigned		ray;
	unsigned		half_wall;
	double			fade;

	ray = 0;
	while (ray < game->img.size.x)
	{
		half_wall = game->column[ray].height / 2;
		if (half_wall > center)
			half_wall = center;
		fade = 4. / (double)game->column[ray].distance;
		if (fade > 1)
			fade = 1;
		if (fade < .01)
			fade = .01;
		draw_line(&game->img, (t_point){ray, center - half_wall},
								(t_point){ray, center + half_wall},
			((int)(0x60 * fade) << 16) + ((int)(0x80 * fade) << 8) + (int)(0x80 * fade));
		ray++;
	}
}

void	get_wall_intersections(t_game *game)
{
	unsigned	ray;
	double		cur_angle;

	ray = 0;
	while (ray < game->img.size.x)
	{
		cur_angle = game->p.angle - POV_ANGLE / 2 * GRAD_TO_RAD +
					POV_ANGLE * ray / (WIN_W - 1) * GRAD_TO_RAD;
		if (cur_angle < 0)
			cur_angle += PI2;
		if (cur_angle > PI2)
			cur_angle -= PI2;
		ray_intersect(game, cur_angle, ray);
		ray++;
	}
}

int	game_loop(t_game *game)
{
	clock_t	start;
	clock_t	end;
	char	*fps;

	start = clock();

	img_clear(&game->img);
//	img_clear_rgb(&game->img, 0x051400);
	img_ceilfloor_fill(&game->img, COLOR_CEIL, COLOR_FLOOR);
//	img_ceilfloor_fill_rgb(&game->img, COLOR_CEIL, COLOR_FLOOR);
	player_control(game);
	get_wall_intersections(game);
	draw_walls(game);
	draw_map(game);
	draw_player(game);
//	fizzlefade(&game->img, 0xFF0000);
//	demo_fillrate(game, 1);
//	demo_radar(game, 360);
//	demo_cursor(game, 0xFF88FF);
	mlx_put_image_to_window(game->mlx, game->win, game->img.ptr, 0, 0);

	end = clock();

//	ft_putnbr_fd(CLOCKS_PER_SEC / (end - start), 1);
//	write(1, "\n", 1);
	fps = ft_itoa(CLOCKS_PER_SEC / (end - start));
	mlx_string_put(game->mlx, game->win, 0, 0, 0xFFFFFF, fps);
	free(fps);
//	write(1, "Player x", 9); ft_putnbr_fd((int)game->p.pos.x, 1);
//	write(1, " y", 3); ft_putnbr_fd((int)game->p.pos.y, 1); write(1, "\n", 1);
	write(1, "Column 0 height ", 16); ft_putnbr_fd(game->column[0].height, 1);
	write(1, " cell x", 7); ft_putnbr_fd((int)game->column[0].cell.x, 1);
	write(1, " y", 2); ft_putnbr_fd((int)game->column[0].cell.y, 1);
	write(1, " dir=", 5); write(1, &(game->column[0].dir), 1); write(1, "\n", 1);
	return (0);
}

int	terminate(void)
{
	exit(EXIT_SUCCESS);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:58:52 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/06 23:41:28 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "x_events.h"

int	main(void)
{
	t_game	mlx;
	int		null;

	mlx = (t_game){0};
	if ((mlx.mlx = mlx_init()) == NULL)
		return (EXIT_FAILURE);
	if (!(mlx.win = mlx_new_window(mlx.mlx, WIN_W, WIN_H, "cub3D by nGragas")))
		return (EXIT_FAILURE);
	if (!(mlx.img.ptr = mlx_new_image(mlx.mlx, WIN_W, WIN_H)))
		return (EXIT_FAILURE);
	mlx.img.data = (int *)mlx_get_data_addr(mlx.img.ptr, &null, &null, &null);
	mlx.img.size = (t_upoint){WIN_W, WIN_H};
	mlx.map.size = (t_upoint){WIN_W, WIN_H};
	mlx_do_key_autorepeatoff(mlx.mlx);
	mlx_hook(mlx.win, EVENT_KEYPRESS, 0, hook_key_press, &mlx.key);
	mlx_hook(mlx.win, EVENT_KEYRELEASE, 0, hook_key_release, &mlx.key);
	mlx_hook(mlx.win, EVENT_BUTTONPRESS, 0, hook_mouse_press, &mlx.key);
	mlx_hook(mlx.win, EVENT_BUTTONRELEASE, 0, hook_mouse_release, &mlx.key);
	mlx_hook(mlx.win, EVENT_MOTIONNOTIFY, 0, hook_mouse_move, &mlx.key);
	mlx_hook(mlx.win, EVENT_DESTROYNOTIFY, 0, terminate, NULL);
	mlx_loop_hook(mlx.mlx, game_loop, &mlx);

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
		mlx.map.grid[i] = map[i];
		i++;
	}
	mlx.map.size = (t_upoint){25, 14};
	mlx.p.pos = (t_fpoint){25 / 2, 14 / 2};
	mlx_loop(mlx.mlx);
}

void	player_control(t_game *game)
{
	if (game->key.k[TURN_LEFT])
		game->p.angle -= 0.1;
	if (game->key.k[TURN_RIGHT])
		game->p.angle += 0.1;
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

t_fpoint	ray_intersect(t_game *game, double cur_angle)
{
	t_fpoint	x_axis;
	t_fpoint	y_axis;
//	t_upoint	len;
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
	step = (cur_angle < M_PI) ?
			(t_fpoint){cos(cur_angle), 1} : (t_fpoint){cos(cur_angle), -1};
	y_axis.y = floor(game->p.pos.y + (step.y > 0));
	y_axis.x = game->p.pos.x + step.x *
			(step.y > 0 ? 1 - (game->p.pos.y - (int)game->p.pos.y) :
								game->p.pos.y - (int)game->p.pos.y);
	while ((unsigned)y_axis.y - (step.y < 0) < game->map.size.y &&
			(unsigned)y_axis.x < game->map.size.x &&
			game->map.grid[(int)y_axis.y - (step.y < 0)][(int)y_axis.x] != '1')
		y_axis = (t_fpoint){y_axis.x + step.x, round(y_axis.y + step.y)};
//	len.x = sqrt(pow(x_axis.x - game->p.pos.x, 2) + pow(x_axis.y - game->p.pos.y, 2));
//	len.y = sqrt(pow(y_axis.x - game->p.pos.x, 2) + pow(y_axis.y - game->p.pos.y, 2));
	return (y_axis);
//	return (len.x < len.y ? x_axis : y_axis);
}

void	draw_player(t_game *game)
{
	int			i;
	t_fpoint	intersect;
	double		cur_angle;

	draw_square(&game->img, (t_point){game->p.pos.x * MAP_SCALE,
									game->p.pos.y * MAP_SCALE}, 6, 0xFFFA80);
	i = 0;
	while (i < RAYS)
	{
		cur_angle = game->p.angle - POV_ANGLE / 2 * GRAD_TO_RAD +
				POV_ANGLE * i / (RAYS - 1) * GRAD_TO_RAD;
		intersect = ray_intersect(game, cur_angle);
//		intersect = (t_fpoint){game->p.pos.x + MAP_RAYLEN / MAP_SCALE * cos(cur_angle),
//							  game->p.pos.y + MAP_RAYLEN / MAP_SCALE * sin(cur_angle)};
		draw_line(&game->img,
			(t_point){game->p.pos.x * MAP_SCALE, game->p.pos.y * MAP_SCALE},
			(t_point){intersect.x * MAP_SCALE, intersect.y * MAP_SCALE}, 0x888015);
		i++;
	}
	draw_line(&game->img, (t_point){game->p.pos.x * MAP_SCALE,
									game->p.pos.y * MAP_SCALE}, (t_point){
		game->p.pos.x * MAP_SCALE + MAP_SCALE * cos(game->p.angle),
		game->p.pos.y * MAP_SCALE + MAP_SCALE * sin(game->p.angle)}, 0xFF4020);
}

int	game_loop(t_game *game)
{
	clock_t	start;
	clock_t	end;
	char	*fps;

	start = clock();

//	img_clear(&game->img);
	img_clear_rgb(&game->img, 0x051400);
	player_control(game);
	draw_map(game);
	draw_player(game);
//	fizzlefade(&game->img, 0xFF0000);
//	img_ceilfloor_fill(&game->img, COLOR_CEIL, COLOR_FLOOR);
//	img_ceilfloor_fill_rgb(&game->img, COLOR_CEIL, COLOR_FLOOR);
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
	write(1, "Player x", 9); ft_putnbr_fd((int)game->p.pos.x, 1);
	write(1, " y", 3); ft_putnbr_fd((int)game->p.pos.y, 1); write(1, "\n", 1);
	return (0);
}

int	terminate(void)
{
	exit(EXIT_SUCCESS);
}
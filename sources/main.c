/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:58:52 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/22 18:38:14 by ngragas          ###   ########.fr       */
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
	if (!(game.win = mlx_new_window(game.mlx, WIN_W, WIN_H, "cub3D. nGragas")))
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

	game.map.grid = (char *[]){	"1111111111111111111111111",
								"1000000000110000000000000",
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
	game.map.size = (t_upoint){25, 14};
	game.p.pos = (t_fpoint){12, 3.5};
	game.p.angle = 0;

	if (!(game.wall[WALL_N].ptr = mlx_xpm_file_to_image(game.mlx, "resources/wall_1.xpm",
					(int *)&game.wall[WALL_N].size.x, (int *)&game.wall[WALL_N].size.y)))
		return (EXIT_FAILURE);
	game.wall[WALL_N].data = (int *)mlx_get_data_addr(game.wall[WALL_N].ptr, &null, &null, &null);
	if (!(game.wall[WALL_S].ptr = mlx_xpm_file_to_image(game.mlx, "resources/wall_2.xpm",
					(int *)&game.wall[WALL_S].size.x, (int *)&game.wall[WALL_S].size.y)))
		return (EXIT_FAILURE);
	game.wall[WALL_S].data = (int *)mlx_get_data_addr(game.wall[WALL_S].ptr, &null, &null, &null);
	if (!(game.wall[WALL_W].ptr = mlx_xpm_file_to_image(game.mlx, "resources/wall_3.xpm",
					(int *)&game.wall[WALL_W].size.x, (int *)&game.wall[WALL_W].size.y)))
		return (EXIT_FAILURE);
	game.wall[WALL_W].data = (int *)mlx_get_data_addr(game.wall[WALL_W].ptr, &null, &null, &null);
	if (!(game.wall[WALL_E].ptr = mlx_xpm_file_to_image(game.mlx, "resources/wall_4.xpm",
					(int *)&game.wall[WALL_E].size.x, (int *)&game.wall[WALL_E].size.y)))
		return (EXIT_FAILURE);
	game.wall[WALL_E].data = (int *)mlx_get_data_addr(game.wall[WALL_E].ptr, &null, &null, &null);

	if (!(game.sprite[0].img.ptr = mlx_xpm_file_to_image(game.mlx, "resources/sprite_3.xpm",
					(int *)&game.sprite[0].img.size.x, (int *)&game.sprite[0].img.size.y)))
		return (EXIT_FAILURE);
	game.sprite[0].img.data = (int *)mlx_get_data_addr(game.sprite[0].img.ptr, &null, &null, &null);
	game.sprite[0].frames = 1;
	game.sprite[0].aspect = game.sprite[0].img.size.x /
													game.sprite[0].img.size.y;
	game.object[0].pos = (t_fpoint){23.95, 3.5};
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
		game->p.pos.x = 0.0001;
	if (game->p.pos.y < 0)
		game->p.pos.y = 0.0001;
	if (game->p.pos.x >= game->map.size.x)
		game->p.pos.x = game->map.size.x - 0.0001;
	if (game->p.pos.y >= game->map.size.y)
		game->p.pos.y = game->map.size.y - 0.0001;
}

/*
** == ping-pong intersect detection (0.5% faster?)
**void	ray_intersect_n(t_game *game, double cur_angle, unsigned ray)
**{
**	t_fpoint	x_axis;
**	t_fpoint	y_axis;
**	t_fpoint	x_step;
**	t_fpoint	y_step;
**	t_fpoint	dist;
**	t_fpoint	dist_step;
**
**	x_step = (cur_angle < M_PI_2 || cur_angle > 3 * M_PI_2) ?
**		(t_fpoint){1, tan(cur_angle)} : (t_fpoint){-1, -tan(cur_angle)};
**	x_axis.x = (int)game->p.pos.x + (x_step.x > 0) - (x_step.x < 0);
**	x_axis.y = game->p.pos.y + x_step.y * (x_step.x > 0 ? 1 - (game->p.pos.x
**			- (int)game->p.pos.x) : game->p.pos.x - (int)game->p.pos.x);
**	y_step = (cur_angle < M_PI) ?
**		(t_fpoint){1 / tan(cur_angle), 1} : (t_fpoint){-1 / tan(cur_angle), -1};
**	y_axis.y = (int)game->p.pos.y + (y_step.y > 0) - (y_step.y < 0) ;
**	y_axis.x = game->p.pos.x + y_step.x * (y_step.y > 0 ? 1 - (game->p.pos.y
**			- (int)game->p.pos.y) : game->p.pos.y - (int)game->p.pos.y);
**	dist_step.x = cos(game->p.angle) * x_step.x + sin(game->p.angle) * x_step.y;
**	dist_step.y = cos(game->p.angle) * y_step.x + sin(game->p.angle) * y_step.y;
**	dist.x = cos(game->p.angle) * (x_axis.x - game->p.pos.x + (x_step.x < 0)) +
**			 sin(game->p.angle) * (x_axis.y - game->p.pos.y);
**	dist.y = cos(game->p.angle) * (y_axis.x - game->p.pos.x) +
**			 sin(game->p.angle) * (y_axis.y - game->p.pos.y + (y_step.y < 0));
**	while (true)
**	{
**		while (dist.x < dist.y)
**		{
**			if ((unsigned)x_axis.y >= game->map.size.y ||
**				(unsigned)x_axis.x >= game->map.size.x ||
**				game->map.grid[(int)x_axis.y][(int)x_axis.x] == '1')
**			{
**				x_axis.x += (x_step.x < 0);
**				game->column[ray] = (struct s_column){dist.x,
**					COL_SCALE / dist.x, x_axis, x_axis.y - (int)x_axis.y,
**					"EW"[x_axis.x >= game->p.pos.x]};
**				return ;
**			}
**			x_axis = (t_fpoint){x_axis.x + x_step.x, x_axis.y + x_step.y};
**			dist.x += dist_step.x;
**		}
**		while (dist.y < dist.x)
**		{
**			if ((unsigned)y_axis.y >= game->map.size.y ||
**				(unsigned)y_axis.x >= game->map.size.x ||
**				game->map.grid[(int)y_axis.y][(int)y_axis.x] == '1')
**			{
**				y_axis.y += (y_step.y < 0);
**				game->column[ray] = (struct s_column){dist.y,
**					  COL_SCALE / dist.y, y_axis, y_axis.x - (int)y_axis.x,
**					  "NS"[y_axis.y >= game->p.pos.y]};
**				return ;
**			}
**			y_axis = (t_fpoint){y_axis.x + y_step.x, y_axis.y + y_step.y};
**			dist.y += dist_step.y;
**		}
**	}
**}
*/

void	ray_intersect(t_game *game, double cur_angle, unsigned ray)
{
	t_fpoint	x1;
	t_fpoint	y1;
	t_fpoint	distance;
	t_fpoint	step;

	step = (cur_angle < M_PI_2 || cur_angle > 3 * M_PI_2) ?
		(t_fpoint){1, tan(cur_angle)} : (t_fpoint){-1, -tan(cur_angle)};
	x1.x = (int)game->p.pos.x + (step.x > 0) - (step.x < 0);
	x1.y = game->p.pos.y + step.y * (step.x > 0 ? 1 - (game->p.pos.x -
					(int)game->p.pos.x) : game->p.pos.x - (int)game->p.pos.x);
	while ((unsigned)x1.y < game->map.size.y &&
			(unsigned)x1.x < game->map.size.x &&
			game->map.grid[(int)x1.y][(int)x1.x] != '1')
		x1 = (t_fpoint){x1.x + step.x, x1.y + step.y};
	x1.x += (step.x < 0);
	step = (cur_angle < M_PI) ?
		(t_fpoint){1 / tan(cur_angle), 1} : (t_fpoint){-1 / tan(cur_angle), -1};
	y1.y = (int)game->p.pos.y + (step.y > 0) - (step.y < 0);
	y1.x = game->p.pos.x + step.x * (step.y > 0 ? 1 - (game->p.pos.y -
					(int)game->p.pos.y) : game->p.pos.y - (int)game->p.pos.y);
	while ((unsigned)y1.y < game->map.size.y &&
			(unsigned)y1.x < game->map.size.x &&
			game->map.grid[(int)y1.y][(int)y1.x] != '1')
		y1 = (t_fpoint){y1.x + step.x, y1.y + step.y};
	y1.y += (step.y < 0);
	distance.x = cos(game->p.angle) * (x1.x - game->p.pos.x) +
					sin(game->p.angle) * (x1.y - game->p.pos.y);
	distance.y = cos(game->p.angle) * (y1.x - game->p.pos.x) +
					sin(game->p.angle) * (y1.y - game->p.pos.y);
	game->column[ray] = (distance.x < distance.y) ?
		(struct s_column){distance.x, COL_SCALE / distance.x, x1,
							x1.y - (int)x1.y, "EW"[x1.x >= game->p.pos.x]} :
		(struct s_column){distance.y, COL_SCALE / distance.y, y1,
							y1.x - (int)y1.x, "NS"[y1.y >= game->p.pos.y]};
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

int	game_loop(t_game *game)
{
	static clock_t	tick;
	char			*fps;

	player_control(game);
//	img_clear(&game->img);
//	img_clear_rgb(&game->img, 0x051400);
//	img_ceilfloor_fill(&game->img, COLOR_CEIL, COLOR_FLOOR);

	img_ceilfloor_fill_rgb(&game->img, COLOR_CEIL, COLOR_FLOOR);
//	for (int i = 0; i < 1000; ++i)
		get_wall_intersections(game);
	draw_walls(game);
	draw_objects(game);
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

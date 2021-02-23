/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:58:52 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/23 23:23:22 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "cub3d.h"
#include "x_events.h"

int		write_screenshot()
{
	return (EXIT_SUCCESS);
}

void	set_resolution(const char *res_string, t_upoint *res)
{
	if (res->x)
		terminate(ERROR_PARSE, "Duplicated R(esolution) setting in scene file");
	res_string++;
	res_string = atoi_limited(&res->x, res_string, USHRT_MAX);
	if (res_string == NULL)
		terminate(ERROR_PARSE, "R(esolution) X setting is too large (>65535)");
	res_string = atoi_limited(&res->y, res_string, USHRT_MAX);
	if (res_string == NULL)
		terminate(ERROR_PARSE, "R(esolution) Y setting is too large (>65535)");
	if (*res_string != '\0' || res->x == 0 || res->y == 0)
		terminate(ERROR_PARSE, "Wrong R(esolution) setting");
}

void	set_colors(const char *color_string, unsigned *target)
{
	unsigned	r;
	unsigned	g;
	unsigned	b;

	if (*target != (unsigned)-1)
		terminate(ERROR_PARSE, "Duplicated F or C color setting in scene file");
	color_string++;
	color_string = atoi_limited(&r, color_string, UCHAR_MAX);
	if (color_string == NULL)
		terminate(ERROR_PARSE, "F or C color R(ed) value is too large (>255)");
	if (*color_string++ != ',')
		terminate(ERROR_PARSE, "F or C color format: 'F R,G,B' or 'C R,G,B'");
	color_string = atoi_limited(&g, color_string, UCHAR_MAX);
	if (color_string == NULL)
		terminate(ERROR_PARSE, "F or C color R(ed) value is too large (>255)");
	if (*color_string++ != ',')
		terminate(ERROR_PARSE, "F or C color format: 'F R,G,B' or 'C R,G,B'");
	color_string = atoi_limited(&b, color_string, UCHAR_MAX);
	if (color_string == NULL)
		terminate(ERROR_PARSE, "F or C color R(ed) value is too large (>255)");
	*target = (r << 16) | (g << 8) | b;
}

void	set_textures(char *string, t_game *game)
{
	int	id;
	int null;

	if (string[0] == 'S' && string[1] == ' ')
		id = SPRITE;
	else if (string[0] == 'N' && string[1] == 'O')
		id = WALL_N;
	else if (string[0] == 'S' && string[1] == 'O')
		id = WALL_S;
	else if (string[0] == 'W' && string[1] == 'E')
		id = WALL_W;
	else if (string[0] == 'E' && string[1] == 'A')
		id = WALL_E;
	string += 2;
	while (*string == ' ')
		string++;
	if (game->texture[id].ptr != NULL)
		terminate(ERROR_PARSE, "Duplicated texture setting in scene file");
	if (!(game->texture[id].ptr = mlx_xpm_file_to_image(game->mlx, string,
		(int *)&game->texture[id].size.x, (int *)&game->texture[id].size.y)))
		terminate(ERROR_PARSE, "Something went wrong while loading texture");
	game->texture[id].data = (unsigned *)mlx_get_data_addr
								(game->texture[id].ptr, &null, &null, &null);
	game->texture[id].aspect = game->texture[id].size.x /
								game->texture[id].size.y;
}

void	parse_scene(int file_id, char **line, t_game *game)
{
	int	status;

	game->color_floor = -1;
	game->color_ceil = -1;
	while ((status = get_next_line(file_id, line)) >= 0)
	{
		if (**line == 'R')
			set_resolution(*line, &game->img.size);
		else if (**line == 'C')
			set_colors(*line, &game->color_ceil);
		else if (**line == 'F')
			set_colors(*line, &game->color_floor);
		else if (**line == 'N' || **line == 'S' ||
				**line == 'W' || **line == 'E')
			set_textures(*line, game);
		else if (**line != '\0')
			return ;
		free(*line);
		if (status == 0)
			terminate(ERROR_PARSE, "There is no map in scene file");
	}
	if (status == -1)
		terminate(ERROR_PARSE, "Something went wrong while reading scene file");
}

void	parse_map(int file_id, char *line, t_game *game)
{
	t_list		*map;
	int			status;
	unsigned	line_len;
	unsigned	i;

	map = ft_lstnew(line);
	game->map.size = (t_upoint){ft_strlen(line), 1};
	while ((status = get_next_line(file_id, &line)) >= 0 && *line != '\0')
	{
		ft_lstadd_front(&map, ft_lstnew(line));
		if ((line_len = ft_strlen(line)) > game->map.size.x)
			game->map.size.x = line_len;
		game->map.size.y++;
	}
	if (status == -1)
		terminate(ERROR_PARSE, "Something went wrong while reading scene file");
	free(line);
	if (status != 0)
		terminate(ERROR_PARSE, "Empty lines in map content are not allowed");
	if ((game->map.grid = malloc(sizeof(char *) * game->map.size.y)) == NULL)
		terminate(ERROR_MEMORY, "Memory allocation failed (map rows)");
	i = game->map.size.y;
	while (i > 0)
	{
		i--;
		if ((game->map.grid[i] = malloc(game->map.size.x)) == NULL)
			terminate(ERROR_MEMORY, "Memory allocation failed (map cols)");
		line = ft_lstpop(&map);
		line_len = ft_strlcpy(game->map.grid[i], line, game->map.size.x);
		ft_memset(game->map.grid[i] + line_len, ' ', game->map.size.x - line_len);
		free(line);
	}
}

void	parse_file(int args, char *av[], t_game *game)
{
	int		file_id;
	char	*line;

	if (args == 1)
		terminate(ERROR_ARGS, "Please specify scene filename");
	else if (args >= 4)
		terminate(ERROR_ARGS, "Too many arguments");
	av++;
	if (ft_strlen(*av) < 5 || ft_memcmp(".cub", *av + ft_strlen(*av) - 4, 5))
		terminate(ERROR_ARGS, "Wrong scene filename");
	if ((file_id = open(*av, O_RDONLY)) == -1)
		terminate(ERROR_ARGS, strerror(errno));
	parse_scene(file_id, &line, game);
	parse_map(file_id, line, game);
	if (close(file_id) == -1)
		terminate(ERROR_PARSE, strerror(errno));
	if (args == 3)
	{
		if (ft_strncmp("--save", *++av, 7) == 0)
			write_screenshot();
		else
			terminate(ERROR_ARGS, "Invalid option");
	}
}

void	initialize(t_game *game)
{
	t_upoint	max_res = {2000, 1200};
	t_upoint	min_res = {2, 1};
	int			n;

	if (game->img.size.x > max_res.x)
		game->img.size.x = max_res.x;
	if (game->img.size.y > max_res.y)
		game->img.size.y = max_res.y;
	if (game->img.size.x < min_res.x)
		game->img.size.x = min_res.x;
	if (!(game->win = mlx_new_window(
			game->mlx, game->img.size.x, game->img.size.y, WINDOW_TITLE)))
		terminate(ERROR_MLX, strerror(errno));
	if (!(game->img.ptr = mlx_new_image(
			game->mlx, game->img.size.x, game->img.size.y)))
		terminate(ERROR_MLX, strerror(errno));
	game->img.data = (unsigned *)mlx_get_data_addr(game->img.ptr, &n, &n, &n);
}

int	main(int args, char *av[])
{
	t_game	game;
	int		null;

	errno = 0;
	game = (t_game){0};
	if (!(game.mlx = mlx_init()))
		terminate(ERROR_MLX, strerror(errno));
	parse_file(args, av, &game);
	initialize(&game);
	ft_putendl_fd("OK!", 1);
	exit(0);
	game.map.size = (t_upoint){25, 14};
	game.map.img.size = (t_upoint){game.map.size.x * MAP_SCALE,
								game.map.size.y * MAP_SCALE};
	if (!(game.map.img.ptr = mlx_new_image(game.mlx,
							game.map.img.size.x, game.map.img.size.y)))
		return (EXIT_FAILURE);
	game.map.img.data = (unsigned *)mlx_get_data_addr(game.map.img.ptr, &null, &null,
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

	game.object[0].pos = (t_fpoint){23.95, 3.5};
	game.object[0].sprite = &game.texture[SPRITE];
	game.object[1].pos = (t_fpoint){21.5, 3.5};
	game.object[1].sprite = &game.texture[SPRITE];
	game.object[2].pos = (t_fpoint){22, 3.2};
	game.object[2].sprite = &game.texture[SPRITE];
	game.object[3].pos = (t_fpoint){22, 3};
	game.object[3].sprite = &game.texture[SPRITE];
	game.object_count = 4;
	mlx_loop(game.mlx);
}

int	game_loop(t_game *game)
{
	static clock_t	tick;
	char			*fps;

	player_control(game);
	img_ceilfloor_fill_rgb(&game->img, game->color_ceil, game->color_floor);
//	for (int i = 0; i < 1000; ++i)
	ray_cast(game);
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

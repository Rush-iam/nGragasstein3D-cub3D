/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:59 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/24 23:17:50 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	parse(int args, char **av, t_game *game, bool *screenshot_only)
{
	int		file_id;
	char	*line;

	if (args == 1)
		terminate(game, ERR_ARGS, "Please specify scene filename");
	else if (args >= 4)
		terminate(game, ERR_ARGS, "Too many arguments");
	av++;
	if (ft_strlen(*av) < 5 || ft_memcmp(".cub", *av + ft_strlen(*av) - 4, 5))
		terminate(game, ERR_ARGS, "Wrong scene filename");
	if ((file_id = open(*av, O_RDONLY)) == -1)
		terminate(game, ERR_ARGS, strerror(errno));
	parse_scene(file_id, &line, game);
	validate_settings(game);
	parse_map(file_id, line, game->objects, game);
	if (close(file_id) == -1)
		terminate(game, ERR_PARSE, strerror(errno));
	*screenshot_only = false;
	if (args == 3)
	{
		if (ft_strncmp("--save", *(av + 1), 7) == 0)
			*screenshot_only = true;
		else
			terminate(game, ERR_ARGS, "Invalid option");
	}
}

void	set_enemies(char *string, t_game *g)
{
	unsigned	id;
//	int			i;
	char		*path;

	if ((string = atoi_limited(&id, string + 1, 100)) == NULL)
		terminate(g, ERR_PARSE, "Enemy ID is wrong (Exx)");
	if (id >= sizeof(g->enemyset) / sizeof(*g->enemyset))
		terminate(g, ERR_PARSE, "Enemy ID out of array range");
	if (g->enemyset[id].death[0].ptr != NULL)
		terminate(g, ERR_PARSE, "Duplicated enemy spriteset setting");
	if ((path = ft_strjoin(string, "wait_")) == NULL)
		terminate(g, ERR_PARSE, strerror(errno));
	load_spriteset(g->enemyset[id].wait, 8, path, g);
	if ((path = ft_strjoin(string, "attack_")) == NULL)
		terminate(g, ERR_PARSE, strerror(errno));
	load_spriteset(g->enemyset[id].attack, 3, path, g);
	if ((path = ft_strjoin(string, "dead_")) == NULL)
		terminate(g, ERR_PARSE, strerror(errno));
	load_spriteset(g->enemyset[id].death, 5, path, g);
	if ((path = ft_strjoin(string, "pain_")) == NULL)
		terminate(g, ERR_PARSE, strerror(errno));
	load_spriteset(g->enemyset[id].pain, 2, path, g);
	load_audioset(&g->enemyset[id], string, g);
//	i = 0;
//	while (i < 4)
//	{
//		if (!(texture->ptr = mlx_png_file_to_image(game->mlx, path,
//							(int *)&texture->size.x, (int *)&texture->size.y)))
//			terminate(game, ERR_PARSE, "Can't load weapon texture file");
//		texture->data = (unsigned *)mlx_get_data_addr(texture->ptr, &n, &n, &n);
//		texture->aspect = texture->size.x / texture->size.y;
//		i++;
//	}
}

void	set_audio(char *string, t_game *game)
{
	unsigned	id;

	if (*string == 'M')
	{
		if ((string = atoi_limited(&id, string + 1, 100)) == NULL)
			terminate(game, ERR_PARSE, "Music ID is wrong (Mxx)");
		if (id >= sizeof(game->audio.music) / sizeof(*game->audio.music))
			terminate(game, ERR_PARSE, "Music ID out of array range");
		if (game->audio.music[id].channels[0] != NULL)
			terminate(game, ERR_PARSE, "Duplicated music setting");
		game->audio.music[id] = load_audio_file(string);
	}
	else if (*string == 'A')
	{
		if ((string = atoi_limited(&id, string + 1, 100)) == NULL)
			terminate(game, ERR_PARSE, "Sound ID is wrong (Axx)");
		if (id >= sizeof(game->audio.sound) / sizeof(*game->audio.sound))
			terminate(game, ERR_PARSE, "Sound ID out of array range");
		if (game->audio.sound[id].channels[0] != NULL)
			terminate(game, ERR_PARSE, "Duplicated sound setting");
		game->audio.sound[id] = load_audio_file(string);
	}
}

void	parse_scene(int file_id, char **line, t_game *game)
{
	int	status;

	game->color_floor = -1U;
	game->color_ceil = -1U;
	while ((status = get_next_line(file_id, line)) >= 0)
	{
		if (**line == 'R')
			set_resolution(*line, &game->img.size, game);
		else if (**line == 'C')
			set_colors(*line, &game->color_ceil, game);
		else if (**line == 'F')
			set_colors(*line, &game->color_floor, game);
		else if (**line == 'W' || **line == 'S')
			set_textures(*line, game);
		else if (**line == 'G')
			set_weapons(*line, game);
		else if (**line == 'E')
			set_enemies(*line, game);
		else if (**line == 'M' || **line == 'A')
			set_audio(*line, game);
		else if (**line != '#' && **line != '\0')
			return ;
		free(*line);
		if (status == 0)
			terminate(game, ERR_PARSE, "There is no map in scene file");
	}
	if (status == -1)
		terminate(game, ERR_PARSE, "Can't load scene file");
}

void	parse_map(int file_id, char *line, t_list *map, t_game *game)
{
	t_list		*line_lst;
	int			status;
	unsigned	line_len;

	if ((map = ft_lstnew(line)) == NULL)
		terminate(game, ERR_MEM, "Memory allocation failed (map first row)");
	game->map.size = (t_upoint){ft_strlen(line), 1};
	while ((status = get_next_line(file_id, &line)) >= 0 && *line != '\0')
	{
		if ((line_lst = ft_lstnew(line)) == NULL)
			terminate(game, ERR_MEM, "Memory allocation failed (map rows)");
		ft_lstadd_front(&map, line_lst);
		if (game->map.size.x < (line_len = ft_strlen(line)))
			game->map.size.x = line_len;
		game->map.size.y++;
	}
	if (status == -1)
		terminate(game, ERR_PARSE, "Can't load scene file");
	free(line);
	if (status != 0)
		terminate(game, ERR_PARSE, "Empty lines in map are not allowed");
	if ((game->map.grid = malloc(sizeof(char *) * game->map.size.y)) == NULL)
		terminate(game, ERR_MEM, "Memory allocation failed (map rows)");
	set_map(game, map);
}

void	validate_settings(t_game *game)
{
	unsigned	i;

	if (game->color_floor == -1U)
		terminate(game, ERR_PARSE, "Floor color not found. Format: F R,G,B");
	if (game->color_ceil == -1U)
		terminate(game, ERR_PARSE, "Ceil color not found. Format: C R,G,B");
	if (game->img.size.x == 0 || game->img.size.y == 0)
		terminate(game, ERR_PARSE,
		"Resolution doesn't set. Format: 'R WIDTH HEIGHT'");
	i = 0;
	while (i < sizeof(game->texture) / sizeof(*game->texture) / 2)
		if (game->texture[i++].ptr == NULL)
			terminate(game, ERR_PARSE, "Missing wall texture");
	i = 0;
	while (i < sizeof(game->sprite) / sizeof(*game->sprite))
		if (game->sprite[i++].ptr == NULL)
			terminate(game, ERR_PARSE, "Missing sprite");
	i = 0;
	while (i < sizeof(game->p.weapon_img) / sizeof(*game->p.weapon_img))
		if (game->p.weapon_img[i++][0].ptr == NULL)
			terminate(game, ERR_PARSE, "Missing weapon texture");
}

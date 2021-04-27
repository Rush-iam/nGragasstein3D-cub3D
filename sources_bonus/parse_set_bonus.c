/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_set_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:55 by ngragas           #+#    #+#             */
/*   Updated: 2021/04/05 19:10:57 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	set_resolution(const char *res_string, t_upoint *res, t_game *game)
{
	if (res->x)
		terminate(game, ERR_PARSE, "Duplicated Resolution setting");
	res_string++;
	res_string = atoi_limited(&res->x, res_string, UINT_MAX);
	if (res_string == NULL)
		terminate(game, ERR_PARSE, "Resolution X setting is wrong");
	res_string = atoi_limited(&res->y, res_string, UINT_MAX);
	if (res_string == NULL)
		terminate(game, ERR_PARSE, "Resolution Y setting is wrong");
	else if (*res_string != '\0' || res->x == 0 || res->y == 0)
		terminate(game, ERR_PARSE, "Wrong Resolution setting");
}

void	set_colors(const char *color_string, unsigned *target, t_game *game)
{
	unsigned	r;
	unsigned	g;
	unsigned	b;

	if (*target != (unsigned)-1)
		terminate(game, ERR_PARSE, "Duplicated F or C color setting");
	color_string++;
	color_string = atoi_limited(&r, color_string, UCHAR_MAX);
	if (color_string == NULL)
		terminate(game, ERR_PARSE, "F/C color Red is wrong (range: 0-255)");
	else if (*color_string++ != ',')
		terminate(game, ERR_PARSE, "F/C color format: 'F R,G,B'/'C R,G,B'");
	color_string = atoi_limited(&g, color_string, UCHAR_MAX);
	if (color_string == NULL)
		terminate(game, ERR_PARSE, "F/C color Green is wrong (range: 0-255)");
	else if (*color_string++ != ',')
		terminate(game, ERR_PARSE, "F/C color format: 'F R,G,B'/'C R,G,B'");
	color_string = atoi_limited(&b, color_string, UCHAR_MAX);
	if (color_string == NULL)
		terminate(game, ERR_PARSE, "F/C color Blue is wrong (range: 0-255)");
	else if (*color_string != '\0')
		terminate(game, ERR_PARSE, "F/C color line redundant symbols");
	*target = (r << 16) | (g << 8) | b;
}


void	set_weapons(char *string, t_game *game)
{
	unsigned	id;
	int			i;
	char		*path;

	if ((string = atoi_limited(&id, string + 1, 100)) == NULL)
		terminate(game, ERR_PARSE, "Weapon ID is wrong (Gx)");
	if (id >= sizeof(game->p.weapon_img) / sizeof(*game->p.weapon_img))
		terminate(game, ERR_PARSE, "Weapon ID out of array range");
	if (game->p.weapon_img[id][0].ptr != NULL)
		terminate(game, ERR_PARSE, "Duplicated weapon setting");
	i = 0;
	while (i < 4)
	{
		if ((path = ft_strjoin(string,
						(char []){'0' + i, '.', 'p', 'n', 'g', '\0'})) == NULL)
			terminate(game, ERR_PARSE, strerror(errno));
		img_from_file(game, path, &game->p.weapon_img[id][i],
					  "Can't load weapon texture file");
		free(path);
		i++;
	}
}

void	set_textures(char *string, t_game *game)
{
	unsigned	id;

	if (*string == 'W')
	{
		if ((string = atoi_limited(&id, string + 1, 100)) == NULL)
			terminate(game, ERR_PARSE, "Wall ID is wrong");
		if (id >= sizeof(game->texture) / sizeof(*game->texture) / 2)
			terminate(game, ERR_PARSE, "Texture ID out of array range");
		if (game->texture[id].ptr != NULL)
			terminate(game, ERR_PARSE, "Duplicated texture setting");
		img_from_file(game, string, &game->texture[id],
					  "Can't load wall texture file");
	}
	else if (*string == 'S')
	{
		if ((string = atoi_limited(&id, string + 1, 100)) == NULL)
			terminate(game, ERR_PARSE, "Sprite ID is wrong");
		if (id >= sizeof(game->sprite) / sizeof(*game->sprite))
			terminate(game, ERR_PARSE, "Sprite ID out of array range");
		if (game->sprite[id].ptr != NULL)
			terminate(game, ERR_PARSE, "Duplicated sprite setting");
		img_from_file(game, string, &game->sprite[id],
					  "Can't load sprite file");
	}
}

void	load_audio_file(t_snd *dst, char *path)
{
	dst->file = cs_load_wav(path);
	if (dst->file.channels[0] == NULL)
		printf("- Failed to load audio %s:\n%s\n", path, cs_error_reason);
	dst->props = cs_make_def(&dst->file);
}

void	load_audioset(t_set *dst, char *path, t_game *game)
{
	char	*path2;
	char	*path3;

	if ((path2 = ft_strjoin(path, "alarm.wav")) == NULL)
		terminate(game, ERR_PARSE, strerror(errno));
	load_audio_file(&dst->s_alarm, path2);
	free(path2);
	if ((path2 = ft_strjoin(path, "attack.wav")) == NULL)
		terminate(game, ERR_PARSE, strerror(errno));
	load_audio_file(&dst->s_attack, path2);
	free(path2);
	if ((path2 = ft_strjoin(path, "death_")) == NULL)
		terminate(game, ERR_PARSE, strerror(errno));
	while (dst->s_death_count < sizeof(dst->s_death) / sizeof(*dst->s_death))
	{
		if ((path3 = ft_strjoin(path2,
		(char []){'0' + dst->s_death_count, '.', 'w', 'a', 'v', 0})) == NULL)
			terminate(game, ERR_PARSE, strerror(errno));
		if (ft_file_exists(path3) == false)
		{
			free(path3);
			break ;
		}
		load_audio_file(&dst->s_death[dst->s_death_count++], path3);
		free(path3);
	}
	free(path2);
}


void	load_spriteset(t_img dst[], int count, char *path, t_game *game)
{
	int		i;
	char	*path2;

	i = 0;
	while (i < count)
	{
		if ((path2 = ft_strjoin(path,
						(char []){'0' + i, '.', 'p', 'n', 'g', '\0'})) == NULL)
			terminate(game, ERR_PARSE, strerror(errno));
		img_from_file(game, path2, &dst[i], "Can't load enemy sprite file");
		free(path2);
		i++;
	}
	free(path);
}

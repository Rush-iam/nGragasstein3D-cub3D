/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_resources_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 16:09:55 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 16:51:46 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	load_walls_and_sprites(char *string, t_game *game)
{
	const uint32_t	m_wall = sizeof(game->texture) / sizeof(*game->texture) / 2;
	const uint32_t	m_sprite = sizeof(game->sprite) / sizeof(*game->sprite);
	uint32_t		id;

	if (*string == 'W')
	{
		string = atoi_limited(&id, string + 1, m_wall);
		if (string == NULL)
			terminate(game, ERR_PARSE, "Texture ID out of array range");
		if (game->texture[id].ptr != NULL)
			terminate(game, ERR_PARSE, "Duplicated texture setting");
		load_image_file(game, string, &game->texture[id], \
										"Can't load wall texture file");
	}
	else if (*string == 'S')
	{
		string = atoi_limited(&id, string + 1, m_sprite);
		if (string == NULL)
			terminate(game, ERR_PARSE, "Sprite ID out of array range");
		if (game->sprite[id].ptr != NULL)
			terminate(game, ERR_PARSE, "Duplicated sprite setting");
		load_image_file(game, string, &game->sprite[id], \
										"Can't load sprite file");
	}
}

void	load_music_and_sounds(char *string, t_game *game)
{
	const uint32_t	max_music_id = sizeof(game->audio.music) / \
									sizeof(*game->audio.music);
	const uint32_t	max_sound_id = sizeof(game->audio.sound) / \
									sizeof(*game->audio.sound);
	uint32_t		id;

	if (*string == 'M')
	{
		string = atoi_limited(&id, string + 1, max_music_id);
		if (string == NULL)
			terminate(game, ERR_PARSE, "Music ID out of array range");
		if (game->audio.music[id].file.channels[0] != NULL)
			terminate(game, ERR_PARSE, "Duplicated music setting");
		load_audio_file(&game->audio.music[id], string);
	}
	else if (*string == 'A')
	{
		string = atoi_limited(&id, string + 1, max_sound_id);
		if (string == NULL)
			terminate(game, ERR_PARSE, "Sound ID out of array range");
		if (game->audio.sound[id].file.channels[0] != NULL)
			terminate(game, ERR_PARSE, "Duplicated sound setting");
		load_audio_file(&game->audio.sound[id], string);
	}
}

void	load_weapons(char *string, t_game *game)
{
	uint32_t	id;
	uint32_t	i;
	char		*path;

	string++;
	if (!ft_isdigit(*string))
		terminate(game, ERR_PARSE, "Weapon ID is wrong (Gx)");
	string = atoi_limited(&id, string, 100);
	if (string == NULL)
		terminate(game, ERR_PARSE, "Weapon ID is wrong (Gx)");
	if (id >= sizeof(game->p.weapon_img) / sizeof(*game->p.weapon_img))
		terminate(game, ERR_PARSE, "Weapon ID out of array range");
	if (game->p.weapon_img[id][0].ptr != NULL)
		terminate(game, ERR_PARSE, "Duplicated weapon setting");
	i = 0;
	while (i < 4)
	{
		path = ft_strjoin(string, (char []){'0' + i, '.', 'p', 'n', 'g', '\0'});
		if (path == NULL)
			terminate(game, ERR_PARSE, strerror(errno));
		load_image_file(game, path, &game->p.weapon_img[id][i], \
									"Can't load weapon texture file");
		free(path);
		i++;
	}
}

void	load_enemyset(char *str, t_game *g)
{
	uint32_t	id;
	char		*path;

	str = atoi_limited(&id, ++str, sizeof(g->enemyset) / sizeof(*g->enemyset));
	if (str == NULL)
		terminate(g, ERR_PARSE, "Enemy ID out of array range");
	if (g->enemyset[id].wait[0].ptr != NULL)
		terminate(g, ERR_PARSE, "Duplicated enemy spriteset setting");
	if (ft_assign_ptr((void *)&path, ft_strjoin(str, "wait_")) == NULL)
		terminate(g, ERR_PARSE, strerror(errno));
	load_spriteset(g->enemyset[id].wait, 8, path, g);
	if (ft_assign_ptr((void *)&path, ft_strjoin(str, "attack_")) == NULL)
		terminate(g, ERR_PARSE, strerror(errno));
	load_spriteset(g->enemyset[id].attack, 3, path, g);
	if (ft_assign_ptr((void *)&path, ft_strjoin(str, "death_")) == NULL)
		terminate(g, ERR_PARSE, strerror(errno));
	load_spriteset(g->enemyset[id].death, 5, path, g);
	if (ft_assign_ptr((void *)&path, ft_strjoin(str, "pain_")) == NULL)
		terminate(g, ERR_PARSE, strerror(errno));
	load_spriteset(g->enemyset[id].pain, 2, path, g);
	load_enemyset_walk(str, id, g);
	load_audioset(&g->enemyset[id], str, g);
}

void	load_enemyset_walk(char *str, uint32_t id, t_game *g)
{
	int		i;
	char	*path;

	i = 0;
	while (i < 8)
	{
		path = ft_strjoin(str, \
					(char []){'w', 'a', 'l', 'k', '_', '0' + i, '\0'});
		if (path == NULL)
			terminate(g, ERR_PARSE, strerror(errno));
		load_spriteset(g->enemyset[id].walk[i], 4, path, g);
		i++;
	}
}

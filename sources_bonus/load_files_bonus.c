/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_files_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 16:16:36 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 15:45:34 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

#ifdef __APPLE__

static void	*png_file_to_image(void *mlx, char *file, int *width, int *height)
{
	return (mlx_png_file_to_image(mlx, file, width, height));
}

#else

static void	*png_file_to_image(void *mlx, char *file, int *width, int *height)
{
	cp_image_t	png_img;
	void		*mlx_img;
	cp_pixel_t 	*mlx_img_data;
	int			i;

	png_img = cp_load_png(file);
	if (png_img.pix == NULL)
		return (NULL);
	cp_premultiply(&png_img);
	mlx_img = new_image(mlx, png_img.w, png_img.h);
	if (mlx_img == NULL)
	{
		free(png_img.pix);
		return (NULL);
	}
	mlx_img_data = (cp_pixel_t *)mlx_get_data_addr(mlx_img, &i, &i, &i);
	i = 0;
	while (i < png_img.w * png_img.h)
	{
		mlx_img_data[i].r = png_img.pix[i].b;
		mlx_img_data[i].g = png_img.pix[i].g;
		mlx_img_data[i].b = png_img.pix[i].r;
		mlx_img_data[i].a = png_img.pix[i].a;
		++i;
	}
	*width = png_img.w;
	*height = png_img.h;
	free(png_img.pix);
	return (mlx_img);
}

#endif

void	load_image_file(t_game *g, char *path, t_img *dst_img, char *err)
{
	int				n;
	const size_t	str_len = ft_strlen(path);

	if (str_len < 5)
		terminate(g, ERR_ARGS, "Can't identify texture format (.xpm/.png)");
	if (ft_memcmp(".xpm", path + str_len - 4, 5) == 0)
		dst_img->ptr = mlx_xpm_file_to_image(g->mlx, path, \
						(int *)&dst_img->size.x, (int *)&dst_img->size.y);
	else if (ft_memcmp(".png", path + str_len - 4, 5) == 0)
		dst_img->ptr = png_file_to_image(g->mlx, path, \
						(int *)&dst_img->size.x, (int *)&dst_img->size.y);
	else
		terminate(g, ERR_ARGS, "Can't identify texture format (.xpm/.png)");
	if (dst_img->ptr == NULL)
		terminate(g, ERR_PARSE, err);
	dst_img->data = (unsigned *)mlx_get_data_addr(dst_img->ptr, &n, &n, &n);
	dst_img->aspect = dst_img->size.x / dst_img->size.y;
	dst_img->alpha_y = ft_calloc(dst_img->size.y, sizeof(bool));
	if (dst_img->alpha_y == NULL)
		terminate(g, ERR_MEM, strerror(errno));
	img_alpha_columns_get(dst_img);
}

void	load_audio_file(t_snd *dst, char *path)
{
	dst->file = cs_load_wav(path);
	if (dst->file.channels[0] == NULL)
		printf("- Failed to load audio %s:\n%s\n", path, cs_error_reason);
	dst->props = cs_make_def(&dst->file);
}

void	load_spriteset(t_img *dst, int count, char *path, t_game *game)
{
	int		i;
	char	*path2;

	i = 0;
	while (i < count)
	{
		path2 = ft_strjoin(path, (char []){'0' + i, '.', 'p', 'n', 'g', '\0'});
		if (path2 == NULL)
			terminate(game, ERR_PARSE, strerror(errno));
		load_image_file(game, path2, &dst[i], "Can't load enemy sprite file");
		free(path2);
		i++;
	}
	free(path);
}

void	load_audioset(t_set *dst, char *path, t_game *game)
{
	char	*path2;

	path2 = ft_strjoin(path, "alarm.wav");
	if (path2 == NULL)
		terminate(game, ERR_PARSE, strerror(errno));
	load_audio_file(&dst->s_alarm, path2);
	free(path2);
	path2 = ft_strjoin(path, "attack.wav");
	if (path2 == NULL)
		terminate(game, ERR_PARSE, strerror(errno));
	load_audio_file(&dst->s_attack, path2);
	free(path2);
	path2 = ft_strjoin(path, "death_");
	if (path2 == NULL)
		terminate(game, ERR_PARSE, strerror(errno));
	load_audioset_multiple(dst, path2, game);
	free(path2);
}

void	load_audioset_multiple(t_set *dst, char *path, t_game *game)
{
	char	*path3;

	while (dst->s_death_count < sizeof(dst->s_death) / sizeof(*dst->s_death))
	{
		path3 = ft_strjoin(path, \
					(char []){'0' + dst->s_death_count, '.', 'w', 'a', 'v', 0});
		if (path3 == NULL)
			terminate(game, ERR_PARSE, strerror(errno));
		if (ft_file_exists(path3) == false)
		{
			free(path3);
			break ;
		}
		load_audio_file(&dst->s_death[dst->s_death_count++], path3);
		free(path3);
	}
}

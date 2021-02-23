/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 16:20:11 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/23 19:44:37 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_point	points_sum(t_point p1, t_point p2)
{
	return (t_point){p1.x + p2.x, p1.y + p2.y};
}

char	*atoi_limited(unsigned *dst_int, const char *src_string, unsigned limit)
{
	while (*src_string == ' ')
		src_string++;
	while (ft_isdigit(*src_string))
	{
		*dst_int = *dst_int * 10 + *src_string++ - '0';
		if (*dst_int > limit)
			return (NULL);
	}
	while (*src_string == ' ')
		src_string++;
	return ((char *)src_string);
}

int		terminate(int return_value, char *message)
{
	if (return_value)
	{
		ft_putendl_fd("Error", 2);
		ft_putendl_fd(message, 2);
		if (return_value == ERROR_MLX)
			ft_putendl_fd("MLX/X11 crashed", 2);
		if (return_value == ERROR_PARSE)
			ft_putendl_fd("Plese fix scene file", 2);
		if (return_value == ERROR_ARGS)
			ft_putendl_fd("Usage: ./cub3D scene_name.cub [--save]", 2);
	}
	exit(return_value);
}

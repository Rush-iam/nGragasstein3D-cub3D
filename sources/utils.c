/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 16:20:11 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/22 23:22:06 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_point	points_sum(t_point p1, t_point p2)
{
	return (t_point){p1.x + p2.x, p1.y + p2.y};
}

int		terminate(int return_value, char *message)
{
	if (return_value)
	{
		ft_putendl_fd("Error", 2);
		ft_putendl_fd(message, 2);
		if (return_value == ERROR_PARSE)
			ft_putendl_fd("Plese fix scene file", 2);
		if (return_value == ERROR_ARGS)
			ft_putendl_fd("Usage: ./cub3D scene_name.cub [--save]", 2);
	}
	exit(return_value);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:57:18 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/04 22:50:28 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <mlx.h>
# include <math.h>
# include "libft.h"
# include "time.h"

# define WIN_W 1280
# define WIN_H 720

enum	e_keys
{
	K_ESCAPE = 53
};

typedef struct
{
	int	x;
	int	y;
}		t_point;

typedef struct
{
	void	*ptr;
	int		*data;
	t_point	size;
}			t_img;

typedef struct
{
	void	*mlx;
	void	*win;
	t_img	buf;
}			t_mlx;

#endif

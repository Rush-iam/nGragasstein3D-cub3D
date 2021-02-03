/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:57:18 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/03 21:27:48 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <mlx.h>
# include "libft.h"
# include "time.h"
# include "math.h"

#define WIN_W 1280
#define WIN_H 720

typedef struct	s_mlx
{
	void*		mlx;
	void*		win;
}				t_mlx;

typedef struct	s_point
{
	int			x;
	int			y;
}				t_point;

//typedef union		s_color
//{
//	unsigned		argb;
//	struct
//	{
//		unsigned	b : 8;
//		unsigned	g : 8;
//		unsigned	r : 8;
//		unsigned	a : 8;
//	};
//}					t_color;

#endif
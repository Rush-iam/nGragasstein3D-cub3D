/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 22:16:42 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/05 23:04:35 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "x_events.h"

int	hook_key_press(int key, t_mlx *mlx)
{
	(void)mlx;
	ft_putstr_fd("\nkeyboard = ", 1);
	ft_putnbr_fd(key, 1);
	if (key == KEY_ESCAPE)
		terminate();
	return (0);
}

int	hook_key_release(int key, t_mlx *mlx)
{
	(void)mlx;
	(void)key;
	return (0);
}

int	hook_mouse_press(int button, int x, int y, t_mlx *mlx)
{
	ft_putstr_fd("\nmouse pr = ", 1);
	ft_putnbr_fd(button, 1);
	write(1, " ", 1);
	ft_putnbr_fd(x, 1);
	write(1, " ", 1);
	ft_putnbr_fd(y, 1);
	pixel_put(&mlx->buf, x, y, 0xFFFFFF);
	return (0);
}

int	hook_mouse_release(int button, int x, int y, t_mlx *mlx)
{
	ft_putstr_fd("\nmouse re = ", 1);
	ft_putnbr_fd(button, 1);
	write(1, " ", 1);
	ft_putnbr_fd(x, 1);
	write(1, " ", 1);
	ft_putnbr_fd(y, 1);
	pixel_put(&mlx->buf, x, y, 0xFFFFFF);
	return (0);
}

int	hook_mouse_move(int x, int y, t_mlx *mlx)
{
	ft_putstr_fd("\nmouse = ", 1);
	ft_putnbr_fd(x, 1);
	write(1, " ", 1);
	ft_putnbr_fd(y, 1);
	pixel_put(&mlx->buf, x, y, 0xFFFFFF);
	return (0);
}

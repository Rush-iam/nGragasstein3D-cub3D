/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minmax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 15:06:26 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/17 15:06:26 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_min(int a, int b)
{
	if (a < b)
		return (a);
	else
		return (b);
}

int	ft_max(int a, int b)
{
	if (a > b)
		return (a);
	else
		return (b);
}

uint32_t	ft_umin(uint32_t a, uint32_t b)
{
	if (a < b)
		return (a);
	else
		return (b);
}

uint32_t	ft_umax(uint32_t a, uint32_t b)
{
	if (a > b)
		return (a);
	else
		return (b);
}

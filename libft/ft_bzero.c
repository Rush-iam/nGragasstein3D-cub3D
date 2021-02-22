/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 19:13:21 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/03 16:45:32 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	if (!n)
		return ;
	*(char *)s++ = 0;
	while (--n)
		*(char *)s++ = 0;
}

/*
**void	ft_bzero(void *s, size_t n)
**{
**	if (!n)
**		return ;
**	*(char *)s = 0;
**	*(char *)(s + n - 1) = 0;
**	if (n <= 2)
**		return ;
**	*(short *)(s + 1) = 0;
**	*(short *)(s + n - 3) = 0;
**	if (n <= 6)
**		return ;
**	*(int *)(s + 3) = 0;
**	*(int *)(s + n - 7) = 0;
**	if (n <= 14)
**		return ;
**	*(char *)(s + 7) = 0;
**	*(char *)(s + n - 8) = 0;
**	if (n <= 16)
**		return ;
**	s += 8 - (size_t)s % 8;
**	while (n > 8)
**	{
**		*(size_t *)s = 0;
**		s += 8;
**		n -= 8;
**	}
**}
*/

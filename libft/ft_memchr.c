/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 14:46:42 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/02 16:51:19 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*ft_memchr_check_8(const char *s, char c)
{
	if (s[0] == c)
		return ((void *)s);
	if (s[1] == c)
		return ((void *)(s + 1));
	if (s[2] == c)
		return ((void *)(s + 2));
	if (s[3] == c)
		return ((void *)(s + 3));
	if (s[4] == c)
		return ((void *)(s + 4));
	if (s[5] == c)
		return ((void *)(s + 5));
	if (s[6] == c)
		return ((void *)(s + 6));
	return ((void *)(s + 7));
}

static void	*ft_memchr_check_tail(const char *s, char c, size_t n)
{
	if (!n)
		return (NULL);
	while (*(char *)s != c)
	{
		s++;
		if (--n == 0)
			return (NULL);
	}
	return ((void *)s);
}

void	*ft_memchr(const void *s, int c, size_t n)
{
	const size_t	mask_0 = 0x7F7F7F7F7F7F7F7F;
	const int		ch = (unsigned char)c;
	const size_t	mask_c = 0x0101010101010101 * ch;
	size_t			check;

	while (((size_t)s & 0b111) && n)
	{
		if (*(unsigned char *)s++ == ch)
			return ((void *)(s - 1));
		n--;
	}
	while (n >= 8)
	{
		check = *(size_t *)s ^ mask_c;
		if (~(((check & mask_0) + mask_0) | check | mask_0) != 0)
			return (ft_memchr_check_8((char *)s, (char)ch));
		s += 8;
		n -= 8;
	}
	return (ft_memchr_check_tail((char *)s, (char)ch, n));
}

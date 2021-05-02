/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 20:33:05 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/02 17:55:30 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strchr_check_8(const char *s, char c)
{
	if (s[0] == c)
		return ((char *)s);
	if (s[1] == c)
		return ((char *)(s + 1));
	if (s[2] == c)
		return ((char *)(s + 2));
	if (s[3] == c)
		return ((char *)(s + 3));
	if (s[4] == c)
		return ((char *)(s + 4));
	if (s[5] == c)
		return ((char *)(s + 5));
	if (s[6] == c)
		return ((char *)(s + 6));
	return ((char *)(s + 7));
}

char	*ft_strchr(const char *s, int c)
{
	const char		ch = (char)c;
	const size_t	mask_0 = 0x7F7F7F7F7F7F7F7F;
	const size_t	mask_c = 0x0101010101010101 * (unsigned char)ch;
	size_t			check;

	while (*s && ((size_t)s & 0b111))
		if (*s++ == ch)
			return ((char *)(s - 1));
	if (*s == '\0')
		return ((char *)((uintptr_t)s * (ch == '\0')));
	while (~((*(size_t *)s & mask_0) + mask_0 | *(size_t *)s | mask_0) == 0)
	{
		check = *(size_t *)s ^ mask_c;
		if (~((check & mask_0) + mask_0 | check | mask_0) != 0)
			return (ft_strchr_check_8(s, ch));
		s += 8;
	}
	if (*s == ch)
		return ((char *)s);
	while (*s++)
		if (*s == ch)
			return ((char *)s);
	return (NULL);
}

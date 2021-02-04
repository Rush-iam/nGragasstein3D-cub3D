/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 20:33:05 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/02 16:51:46 by ngragas          ###   ########.fr       */
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

char		*ft_strchr(const char *s, int c)
{
	const size_t	mask_0 = 0x7F7F7F7F7F7F7F7F;
	size_t			mask_c;
	size_t			check;

	c = (unsigned char)c;
	while (*s && ((size_t)s & 0b111))
		if (*s++ == c)
			return ((char *)(s - 1));
	if (*s == '\0')
		return (c == '\0' ? (char *)s : NULL);
	mask_c = 0x0101010101010101 * c;
	while (~((*(size_t *)s & mask_0) + mask_0 | *(size_t *)s | mask_0) == 0)
	{
		check = *(size_t *)s ^ mask_c;
		if (~((check & mask_0) + mask_0 | check | mask_0) != 0)
			return (ft_strchr_check_8(s, (char)c));
		s += 8;
	}
	if (*s == c)
		return ((char *)s);
	while (*s++)
		if (*s == c)
			return ((char *)s);
	return (NULL);
}

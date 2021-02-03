/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 14:46:42 by ngragas           #+#    #+#             */
/*   Updated: 2020/12/02 15:12:19 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	while (n > 7)
	{
		if (*(char *)s == (char)c)
			return ((void *)s);
		if (*(char *)(s + 1) == (char)c)
			return ((void *)(s + 1));
		if (*(char *)(s + 2) == (char)c)
			return ((void *)(s + 2));
		if (*(char *)(s + 3) == (char)c)
			return ((void *)(s + 3));
		if (*(char *)(s + 4) == (char)c)
			return ((void *)(s + 4));
		if (*(char *)(s + 5) == (char)c)
			return ((void *)(s + 5));
		if (*(char *)(s + 6) == (char)c)
			return ((void *)(s + 6));
		if (*(char *)(s + 7) == (char)c)
			return ((void *)(s + 7));
		s += 8;
		n -= 8;
	}
	while (n && *(char *)s != (unsigned char)c)
		s++ && n--;
	return (n ? (void *)s : NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 15:45:52 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/03 15:43:35 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_memcmp_check_8(const void *s1, const void *s2)
{
	if (*(char *)s1 != *(char *)s2)
		return (*(unsigned char *)s1 - *(unsigned char *)s2);
	else if (*(char *)(s1 + 1) != *(char *)(s2 + 1))
		return (*(unsigned char *)(s1 + 1) - *(unsigned char *)(s2 + 1));
	else if (*(char *)(s1 + 2) != *(char *)(s2 + 2))
		return (*(unsigned char *)(s1 + 2) - *(unsigned char *)(s2 + 2));
	else if (*(char *)(s1 + 3) != *(char *)(s2 + 3))
		return (*(unsigned char *)(s1 + 3) - *(unsigned char *)(s2 + 3));
	else if (*(char *)(s1 + 4) != *(char *)(s2 + 4))
		return (*(unsigned char *)(s1 + 4) - *(unsigned char *)(s2 + 4));
	else if (*(char *)(s1 + 5) != *(char *)(s2 + 5))
		return (*(unsigned char *)(s1 + 5) - *(unsigned char *)(s2 + 5));
	else if (*(char *)(s1 + 6) != *(char *)(s2 + 6))
		return (*(unsigned char *)(s1 + 6) - *(unsigned char *)(s2 + 6));
	else
		return (*(unsigned char *)(s1 + 7) - *(unsigned char *)(s2 + 7));
}

static int	ft_memcmp_check_16(const void *s1, const void *s2)
{
	if (*(size_t *)s1 ^ *(size_t *)s2)
		return (ft_memcmp_check_8(s1, s2));
	else
		return (ft_memcmp_check_8(s1 + 8, s2 + 8));
}

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	while (n >= 16)
	{
		if (*(__uint128_t *)s1 ^ *(__uint128_t *)s2)
			return (ft_memcmp_check_16(s1, s2));
		s1 += 16;
		s2 += 16;
		n -= 16;
	}
	if (n >= 8)
	{
		if (*(size_t *)s1 ^ *(size_t *)s2)
			return (ft_memcmp_check_8(s1, s2));
		s1 += 8;
		s2 += 8;
		n -= 8;
	}
	if (!n)
		return (0);
	while (--n && *(char *)s1 == *(char *)s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

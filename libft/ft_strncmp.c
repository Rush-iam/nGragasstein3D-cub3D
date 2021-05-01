/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 15:40:23 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/02 19:21:08 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_strncmp_check_tail(const char *s1, const char *s2, size_t n)
{
	if (!n)
		return (0);
	if (*s1 != *s2 || !(n - 1) || !*s1)
		return ((unsigned char)*s1 - (unsigned char)*s2);
	if (*(s1 + 1) != *(s2 + 1) || !(n - 2) || !*(s1 + 1))
		return ((unsigned char)*(s1 + 1) - (unsigned char)*(s2 + 1));
	if (*(s1 + 2) != *(s2 + 2) || !(n - 3) || !*(s1 + 2))
		return ((unsigned char)*(s1 + 2) - (unsigned char)*(s2 + 2));
	if (*(s1 + 3) != *(s2 + 3) || !(n - 4) || !*(s1 + 3))
		return ((unsigned char)*(s1 + 3) - (unsigned char)*(s2 + 3));
	if (*(s1 + 4) != *(s2 + 4) || !(n - 5) || !*(s1 + 4))
		return ((unsigned char)*(s1 + 4) - (unsigned char)*(s2 + 4));
	if (*(s1 + 5) != *(s2 + 5) || !(n - 6) || !*(s1 + 5))
		return ((unsigned char)*(s1 + 5) - (unsigned char)*(s2 + 5));
	if (*(s1 + 6) != *(s2 + 6) || !(n - 7) || !*(s1 + 6))
		return ((unsigned char)*(s1 + 6) - (unsigned char)*(s2 + 6));
	return ((unsigned char)*(s1 + 7) - (unsigned char)*(s2 + 7));
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (!n)
		return (0);
	while (n >= 8)
	{
		if (*s1 != *s2 || !*s1)
			return ((unsigned char)*s1 - (unsigned char)*s2);
		if (*(s1 + 1) != *(s2 + 1) || !*(s1 + 1))
			return ((unsigned char)*(s1 + 1) - (unsigned char)*(s2 + 1));
		if (*(s1 + 2) != *(s2 + 2) || !*(s1 + 2))
			return ((unsigned char)*(s1 + 2) - (unsigned char)*(s2 + 2));
		if (*(s1 + 3) != *(s2 + 3) || !*(s1 + 3))
			return ((unsigned char)*(s1 + 3) - (unsigned char)*(s2 + 3));
		if (*(s1 + 4) != *(s2 + 4) || !*(s1 + 4))
			return ((unsigned char)*(s1 + 4) - (unsigned char)*(s2 + 4));
		if (*(s1 + 5) != *(s2 + 5) || !*(s1 + 5))
			return ((unsigned char)*(s1 + 5) - (unsigned char)*(s2 + 5));
		if (*(s1 + 6) != *(s2 + 6) || !*(s1 + 6))
			return ((unsigned char)*(s1 + 6) - (unsigned char)*(s2 + 6));
		if (*(s1 + 7) != *(s2 + 7) || !*(s1 + 7))
			return ((unsigned char)*(s1 + 7) - (unsigned char)*(s2 + 7));
		s1 += 8;
		s2 += 8;
		n -= 8;
	}
	return (ft_strncmp_check_tail(s1, s2, n));
}

/*
** int	ft_strncmp(const char *s1, const char *s2, size_t n)
** {
** 	if (!n)
** 		return (0);
** 	while (--n && *s1 && *s1 == *s2)
** 	{
** 		s1++;
** 		s2++;
** 	}
** 	return ((unsigned char)*s1 - (unsigned char)*s2);
** }
*/

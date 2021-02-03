/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 20:33:05 by ngragas           #+#    #+#             */
/*   Updated: 2020/11/12 19:27:09 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	c = (char)c;
	if (*s == c)
		return ((char *)s);
	while (*s)
		if (*++s == c)
			return ((char *)s);
	return (NULL);
}

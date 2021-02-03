/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 15:38:41 by ngragas           #+#    #+#             */
/*   Updated: 2020/11/20 14:57:06 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	const char	*start = src;

	if (dstsize && dst)
	{
		while (--dstsize && *src)
			*dst++ = *src++;
		*dst = '\0';
	}
	while (*src)
		src++;
	return (src - start);
}

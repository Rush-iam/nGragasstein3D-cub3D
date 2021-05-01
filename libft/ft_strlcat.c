/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 17:33:07 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/02 21:14:48 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	char	*cur;

	cur = ft_memchr(dst, '\0', dstsize);
	if (cur == NULL)
		return (ft_strlen(src) + dstsize);
	dstsize -= cur - dst;
	while (--dstsize && *src)
		*cur++ = *src++;
	*cur = '\0';
	return (ft_strlen(src) + cur - dst);
}

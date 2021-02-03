/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 21:43:41 by ngragas           #+#    #+#             */
/*   Updated: 2020/11/10 20:20:18 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*cur;

	if (!dst && !src)
		return (NULL);
	if (dst > src)
		while (len--)
			*((char *)dst + len) = *((char *)src + len);
	else
	{
		cur = dst;
		while (len--)
			*cur++ = *(char *)src++;
	}
	return (dst);
}

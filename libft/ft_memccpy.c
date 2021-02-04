/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 19:47:33 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/03 17:07:58 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	const void	*src_char_pos = ft_memchr(src, c, n);

	if (src_char_pos)
	{
		ft_memcpy(dst, src, src_char_pos - src + 1);
		return (dst + (src_char_pos - src + 1));
	}
	else
	{
		ft_memcpy(dst, src, n);
		return (NULL);
	}
}

/*
**	while (n && (*(char *)dst++ = *(char *)src++) != (char)c)
**		n--;
**	return (n ? dst : NULL);
*/

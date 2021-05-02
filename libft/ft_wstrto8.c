/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wstrto8.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 22:14:58 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/01 22:14:58 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	ft_wstrto8
** if dst == NULL: return length in bytes without writing
** if n == -1: SIZE_T_MAX limit (==unlimited)
** if UTF-32 >= 1114112 (0x10FFFF): return error (-1)
*/

size_t	ft_wstrto8(char *dst_utf8, const wchar_t *src_utf32, size_t n)
{
	int		char_size;
	size_t	count;

	count = 0;
	while (*src_utf32 && count < n)
	{
		char_size = 1;
		if (*src_utf32 < 2048)
			char_size = 2;
		else if (*src_utf32 < 65536)
			char_size = 3;
		else if (*src_utf32 < 1114112)
			char_size = 4;
		else if (*src_utf32 >= 1114112)
			return (-1);
		if (count + char_size > n)
			break ;
		count += char_size;
		if (dst_utf8)
			dst_utf8 += ft_wchrto8(dst_utf8, *src_utf32);
		src_utf32++;
	}
	if (dst_utf8)
		*dst_utf8 = '\0';
	return (count);
}

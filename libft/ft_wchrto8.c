/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wchrto8.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 22:03:01 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/02 17:30:57 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	ft_wchrto8
** if UTF-32 >= 1114112 (0x10FFFF): return error (0 bytes)
*/

int	ft_wchrto8(char *dst_utf8, wchar_t src_utf32)
{
	int		bytes;
	char	mask;
	int		i;

	bytes = 0;
	if (++bytes && src_utf32 < 128)
		mask = (char)(0b00000000);
	else if (++bytes && src_utf32 < 2048)
		mask = (char)(0b11000000);
	else if (++bytes && src_utf32 < 65536)
		mask = (char)(0b11100000);
	else if (++bytes && src_utf32 < 1114112)
		mask = (char)(0b11110000);
	else
		return (0);
	dst_utf8[0] = (char)(mask | src_utf32 >> 6 * (bytes - 1));
	i = 0;
	while (++i < bytes)
	{
		dst_utf8[i] = (char)(src_utf32 >> 6 * (bytes - i - 1) & 0b111111);
		dst_utf8[i] |= (char)(0b10000000);
	}
	return (bytes);
}

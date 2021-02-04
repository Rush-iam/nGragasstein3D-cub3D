/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 13:09:30 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/03 16:00:31 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	needle_len;

	if (!*needle)
		return ((char *)haystack);
	needle_len = ft_strlen(needle);
	if (!len || (*haystack && len < needle_len))
		return (NULL);
	len -= needle_len - 1;
	while (len && *haystack)
	{
		if (*haystack == *needle)
			if (!ft_strncmp(haystack + 1, needle + 1, needle_len - 1))
				return ((char *)haystack);
		haystack++;
		len--;
	}
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 13:09:30 by ngragas           #+#    #+#             */
/*   Updated: 2020/11/25 14:22:11 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	const char	*check_haystack;
	const char	*check_needle;
	size_t		needle_len;

	if (!*needle)
		return ((char *)haystack);
	needle_len = ft_strlen(needle);
	if (!len || (*haystack && len < needle_len))
		return (NULL);
	len -= needle_len - 1;
	while (len && *haystack)
	{
		if (*haystack == *needle)
		{
			check_haystack = haystack + 1;
			check_needle = needle + 1;
			while (*check_needle && *check_haystack++ == *check_needle)
				check_needle++;
			if (!*check_needle)
				return ((char *)haystack);
		}
		haystack++;
		len--;
	}
	return (NULL);
}

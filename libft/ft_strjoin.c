/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 15:01:47 by ngragas           #+#    #+#             */
/*   Updated: 2020/11/11 17:09:37 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	char	*cur;
	size_t	len1;
	size_t	len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2) + 1;
	str = malloc(len1 + len2);
	if (str == NULL)
		return (NULL);
	cur = str;
	while (len1--)
		*cur++ = *s1++;
	while (len2--)
		*cur++ = *s2++;
	return (str);
}

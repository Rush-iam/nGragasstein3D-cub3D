/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 20:54:11 by ngragas           #+#    #+#             */
/*   Updated: 2020/12/02 14:16:11 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	char	*cur;

	substr = malloc(++len);
	if (substr == NULL)
		return (NULL);
	cur = substr;
	s += start;
	while (--len)
		*cur++ = *s++;
	*cur = '\0';
	return (substr);
}

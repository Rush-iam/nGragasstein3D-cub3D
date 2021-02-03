/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 18:16:02 by ngragas           #+#    #+#             */
/*   Updated: 2020/12/02 14:14:24 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_fail(char **split, unsigned count)
{
	while (count)
		free(split[--count]);
	free(split);
	return (NULL);
}

char		**ft_split(char const *s, char c)
{
	char const	*cur_read = s;
	unsigned	count;
	char		**split;

	count = 0;
	while (*s)
		if (*s++ != c && ++count)
			while (*s && *s != c)
				s++;
	if (!(split = malloc(sizeof(*split) * (count + 1))))
		return (NULL);
	count = 0;
	while (*cur_read)
		if (*cur_read++ != c)
		{
			s = cur_read - 1;
			while (*cur_read && *cur_read != c)
				cur_read++;
			if (!(split[count++] = ft_substr(s, 0, cur_read - s)))
				return (ft_fail(split, count));
		}
	split[count] = NULL;
	return (split);
}

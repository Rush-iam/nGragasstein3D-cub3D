/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 18:16:02 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/02 17:41:15 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_split(char const *s, char c)
{
	char const		*r = s;
	unsigned int	count;
	char			**spl;

	count = 0;
	while (*s)
		if (*s++ != c && ++count)
			while (*s && *s != c)
				s++;
	if (!ft_assign_ptr((void **)&spl, malloc(sizeof(*spl) * (count + 1))))
		return (NULL);
	count = 0;
	while (*r)
	{
		if (*r++ != c)
		{
			s = r - 1;
			while (*r && *r != c)
				r++;
			if (!ft_assign_ptr((void **)&spl[count++], ft_substr(s, 0, r - s)))
				return (ft_free_ptr_array((void **)spl, count));
		}
	}
	spl[count] = NULL;
	return (spl);
}

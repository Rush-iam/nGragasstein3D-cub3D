/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 15:49:30 by ngragas           #+#    #+#             */
/*   Updated: 2020/11/18 14:25:08 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char const	*start;

	if (!set || !*set)
		return (ft_strdup(s1));
	while (*s1 && ft_strchr(set, *s1))
		s1++;
	if (!*s1)
		return (ft_calloc(1, 1));
	start = s1;
	while (*s1)
		s1++;
	while (ft_strchr(set, *--s1))
		;
	return (ft_substr(start, 0, s1 - start + 1));
}

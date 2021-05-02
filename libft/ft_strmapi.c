/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 19:53:14 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/02 17:25:18 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*newstr;
	unsigned int	index;

	if (!f)
		return (NULL);
	newstr = malloc(ft_strlen(s) + 1);
	if (newstr == NULL)
		return (NULL);
	index = 0;
	while (*s)
	{
		newstr[index] = f(index, *s++);
		index++;
	}
	newstr[index] = '\0';
	return (newstr);
}

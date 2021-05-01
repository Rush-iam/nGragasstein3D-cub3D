/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:32:41 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 18:30:33 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

char	*skip_spaces(char **str)
{
	while (**str == ' ')
		(*str)++;
	return (*str);
}

char	*atoi_limited(uint32_t *dst_int, const char *src_string, uint32_t limit)
{
	unsigned long	num;

	while (*src_string == ' ')
		src_string++;
	if (ft_isdigit(*src_string) == false)
		return (NULL);
	num = 0;
	while (ft_isdigit(*src_string))
	{
		num = num * 10 + *src_string++ - '0';
		if (limit != UINT_MAX && num > limit)
			return (NULL);
		if (num > UINT_MAX)
		{
			while (ft_isdigit(*src_string))
				src_string++;
			num = UINT_MAX;
		}
	}
	*dst_int = ft_umin(num, UINT_MAX);
	while (*src_string == ' ')
		src_string++;
	return ((char *)src_string);
}

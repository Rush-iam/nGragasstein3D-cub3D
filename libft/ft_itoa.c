/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 21:40:07 by ngragas           #+#    #+#             */
/*   Updated: 2020/11/21 20:01:37 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Variable "digits[10]" holds negative sign
*/

char	*ft_itoa(int n)
{
	char			*str;
	unsigned char	i;
	unsigned char	k;
	unsigned char	digits[11];

	digits[10] = n < 0;
	i = !n;
	digits[0] = '0';
	while (n)
	{
		digits[i++] = '0' + n % 10 * ((n > 0) - digits[10]);
		n /= 10;
	}
	str = malloc(digits[10] + i + 1);
	if (str == NULL)
		return (NULL);
	k = digits[10];
	if (k)
		str[0] = '-';
	while (i)
		str[k++] = (char)digits[--i];
	str[k] = '\0';
	return (str);
}

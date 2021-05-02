/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 17:27:46 by ngragas           #+#    #+#             */
/*   Updated: 2020/11/10 19:21:51 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*block;
	size_t	total;

	total = count * size;
	block = malloc(total);
	if (block == NULL)
		return (NULL);
	ft_bzero(block, total);
	return (block);
}

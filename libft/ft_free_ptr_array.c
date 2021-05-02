/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_ptr_array.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 17:12:33 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/02 17:13:36 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_free_ptr_array(void **array, unsigned int count)
{
	unsigned int	i;

	i = 0;
	while (i < count)
		free(array[i++]);
	free(array);
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstremove.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 22:37:36 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/08 22:37:48 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstremove(t_list **begin, t_list *to_delete)
{
	t_list	*prev;
	t_list	*cur;

	cur = *begin;
	prev = NULL;
	while (cur)
	{
		if (cur == to_delete)
		{
			if (prev)
				prev->next = cur->next;
			else
				*begin = cur->next;
			free(cur->content);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

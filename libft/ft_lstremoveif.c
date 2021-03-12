/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstremoveif.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 23:46:59 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/12 23:51:44 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstremoveif(t_list **begin, void *data_ref, int (*cmp)(),
		void (*free_fct)(void *))
{
	t_list	*cur;
	t_list	*next;
	t_list	*prev;

	if (!begin || (begin && !(*begin)) || !data_ref || !(*cmp) || !(*free_fct))
		return ;
	cur = *begin;
	prev = NULL;
	while (cur)
	{
		next = cur->next;
		if ((*cmp)(cur->content, data_ref) == 1)
		{
			if (prev)
				prev->next = next;
			else
				*begin = next;
			free_fct(cur->content);
			free(cur);
		}
		else
			prev = cur;
		cur = next;
	}
}

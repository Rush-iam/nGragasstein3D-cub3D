/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 20:03:25 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/26 22:54:19 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	sort(t_list **begin, t_list *prev, t_list *cur, int (*cmp)())
{
	int		sorted;
	t_list	*next;

	sorted = 1;
	while (cur->next)
	{
		next = cur->next;
		if ((*cmp)(cur->content, next->content) > 0)
		{
			sorted = 0;
			if (prev)
				prev->next = next;
			else
				*begin = next;
			cur->next = next->next;
			next->next = cur;
			prev = next;
		}
		else
		{
			prev = cur;
			cur = next;
		}
	}
	return (sorted);
}

void		ft_lstsort(t_list **begin_list, int (*cmp)())
{
	int	sorted;

	if (!cmp || !*begin_list)
		return ;
	sorted = 0;
	while (!sorted)
		sorted = sort(begin_list, NULL, *begin_list, cmp);
}

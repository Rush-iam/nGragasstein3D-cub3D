/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 20:03:25 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/28 18:39:17 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	sort(t_list **begin, t_list *cur, int (*cmp)())
{
	int		sorted;
	t_list	*prev;
	t_list	*next;

	prev = NULL;
	sorted = 1;
	while ((next = cur->next))
	{
		if ((*cmp)(cur->content, next->content) != 0)
		{
			sorted = 0;
			if (prev)
				prev->next = next;
			else
				*begin = next;
			cur->next = next->next;
			next->next = cur;
			prev = next;
			continue ;
		}
		prev = cur;
		cur = next;
	}
	if (!sorted)
		sort(begin, *begin, cmp);
}

void		ft_lstsort(t_list **lst, int (*cmp)())
{
	if (!cmp || !*lst)
		return ;
	sort(lst, *lst, cmp);
}

/*
**	Linus Tolrvald's elegant sort
**
**void	ft_lstsort(t_list **lst, int (*cmp)())
**{
**	int		sorted;
**	t_list	**cur;
**	t_list	*next;
**
**	if (!cmp || !*lst)
**		return ;
**	sorted = 0;
**	while (!sorted)
**	{
**		sorted = 1;
**		cur = lst;
**		while ((next = (*cur)->next))
**		{
**			if ((*cmp)((*cur)->content, next->content) != 0)
**			{
**				sorted = 0;
**				(*cur)->next = next->next;
**				next->next = *cur;
**				*cur = next;
**			}
**			cur = &(*cur)->next;
**		}
**	}
**}
*/

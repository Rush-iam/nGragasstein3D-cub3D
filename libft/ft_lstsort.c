/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 20:03:25 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/02 16:58:58 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static bool	sort(t_list **begin, t_list *cur, t_list *next, int (*cmp)())
{
	bool	sorted;
	t_list	*prev;

	prev = NULL;
	sorted = true;
	while (next)
	{
		if ((*cmp)(cur->content, next->content) != 0)
		{
			sorted = false;
			if (prev)
				prev->next = next;
			else
				*begin = next;
			cur->next = next->next;
			next->next = cur;
			prev = next;
			next = cur->next;
			continue ;
		}
		prev = cur;
		cur = next;
		next = cur->next;
	}
	return (sorted);
}

void	ft_lstsort(t_list **lst, int (*cmp)())
{
	if (!cmp || !*lst)
		return ;
	while (sort(lst, *lst, (*lst)->next, cmp) == false)
		;
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

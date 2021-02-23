/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstpop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/23 22:52:09 by ngragas           #+#    #+#             */
/*   Updated: 2021/02/23 22:57:28 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_lstpop(t_list **lst)
{
	void	*data;
	t_list	*cur;

	data = (*lst)->content;
	cur = *lst;
	*lst = (*lst)->next;
	free(cur);
	return (data);
}

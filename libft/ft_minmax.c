/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minmax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 15:06:26 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/17 15:06:26 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int			ft_min(int a, int b)
{
	if (a < b)
		return (a);
	else
		return (b);
}

int			ft_max(int a, int b)
{
	if (a > b)
		return (a);
	else
		return (b);
}

unsigned	ft_umin(unsigned a, unsigned b)
{
	if (a < b)
		return (a);
	else
		return (b);
}

unsigned	ft_umax(unsigned a, unsigned b)
{
	if (a > b)
		return (a);
	else
		return (b);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file_exists.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 16:47:45 by ngragas           #+#    #+#             */
/*   Updated: 2021/03/25 16:47:45 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "fcntl.h"

int	ft_file_exists(char *path)
{
	int	fd;
	int	check;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (0);
	check = read(fd, NULL, 0);
	close(fd);
	if (check == -1)
		return (0);
	return (1);
}

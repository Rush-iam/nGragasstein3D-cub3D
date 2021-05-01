/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 16:55:41 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/02 16:35:40 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include "libft.h"

/*
**  MAX_FDS - max limit of open fd-s (Mac OS default soft limit is 256)
**  BIG_BUF - big buffer to hold many read BUFFER_SIZEs
**  BUFFER_SIZE - how many bytes to read from input in one call
*/

# define MAX_FDS 32
# define BIG_BUF 262144
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 65536
# endif

typedef struct s_buf
{
	char		*s;
	char		*cur;
	uint32_t	max;
	uint32_t	cap;
}	t_buf;

int	get_next_line(int fd, char **line);

#endif

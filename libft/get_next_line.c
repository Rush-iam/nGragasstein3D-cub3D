/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 16:59:26 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/02 18:09:19 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	gnl_terminate(int return_value, int fd, char **line, t_buf *buf)
{
	if (return_value == -1)
		*line = NULL;
	else
	{
		*line = ft_substr(buf[fd].cur, 0, buf[fd].max);
		if (*line == NULL)
			return_value = -1;
	}
	if (fd >= 0)
	{
		free(buf[fd].s);
		buf[fd] = (t_buf){NULL, NULL, 0, 0};
	}
	return (return_value);
}

static bool	gnl_buf_realloc(int fd, t_buf *buf)
{
	char	*newbuf;

	if (buf[fd].max + BUFFER_SIZE > buf[fd].cap)
	{
		buf[fd].cap *= 4;
		newbuf = malloc(buf[fd].cap);
		if (newbuf == NULL)
			return (false);
		ft_memcpy(newbuf, buf[fd].cur, buf[fd].max);
		free(buf[fd].s);
		buf[fd].s = newbuf;
	}
	else
		ft_memcpy(buf[fd].s, buf[fd].cur, buf[fd].max);
	buf[fd].cur = buf[fd].s;
	return (true);
}

static int	gnl_fetch_fd(int fd, char **line, t_buf *buf)
{
	ssize_t	bytes_read;
	char	*newline;

	while (true)
	{
		if (buf[fd].cur + buf[fd].max + BUFFER_SIZE - buf[fd].s > buf[fd].cap)
			if (gnl_buf_realloc(fd, buf) == false)
				return (gnl_terminate(-1, fd, line, buf));
		bytes_read = read(fd, buf[fd].cur + buf[fd].max, BUFFER_SIZE);
		if (bytes_read < 1)
			return (gnl_terminate(bytes_read, fd, line, buf));
		newline = ft_memchr(buf[fd].cur + buf[fd].max, '\n', bytes_read);
		if (newline)
		{
			*line = ft_substr(buf[fd].cur, 0, newline - buf[fd].cur);
			if (*line == NULL)
				return (gnl_terminate(-1, fd, line, buf));
			buf[fd].max += buf[fd].cur + bytes_read - newline - 1;
			buf[fd].cur = newline + 1;
			return (1);
		}
		buf[fd].max += bytes_read;
	}
}

int	get_next_line(int fd, char **line)
{
	static t_buf	buf[MAX_FDS];
	char			*newline;

	if (fd < 0)
		return (gnl_terminate(-1, fd, line, buf));
	if (!buf[fd].s)
	{
		buf[fd].cap = BUFFER_SIZE;
		if (BIG_BUF > BUFFER_SIZE)
			buf[fd].cap = BIG_BUF;
		buf[fd].s = malloc(buf[fd].cap);
		if (buf[fd].s == NULL)
			return (gnl_terminate(-1, fd, line, buf));
		buf[fd].cur = buf[fd].s;
		buf[fd].max = 0;
	}
	newline = ft_memchr(buf[fd].cur, '\n', buf[fd].max);
	if (!newline)
		return (gnl_fetch_fd(fd, line, buf));
	*line = ft_substr(buf[fd].cur, 0, newline - buf[fd].cur);
	if (*line == NULL)
		return (gnl_terminate(-1, fd, line, buf));
	buf[fd].max -= newline - buf[fd].cur + 1;
	buf[fd].cur = newline + 1;
	return (1);
}

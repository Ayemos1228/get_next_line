/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomeya <tsomeya@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/23 13:27:34 by tsomeya           #+#    #+#             */
/*   Updated: 2021/04/28 23:47:42 by tsomeya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <libc.h>

char	*ft_strchr(const char *s, int c)
{
	size_t			i;
	size_t			len;
	unsigned char	*ptr;
	unsigned char	c_in;

	if (s == NULL)
		return (NULL);
	i = 0;
	ptr = (unsigned char *)s;
	len = ft_strlen(s);
	c_in = (unsigned char)c;
	while (i <= len)
	{
		if (*ptr == c_in)
			return ((void *)ptr);
		i++;
		ptr++;
	}
	return (NULL);
}

/*
** A function for the case where the variable "stored" has a new line.
** This function returns the subpart of "stored" (up to a new line symbol)
** and rewrite "stored" to include the other part of the string "stored"
**(after a new line symbol until the end)
*/

char	*get_stored_line(char **stored)
{
	char		*ret;
	char		*tmp_ptr;
	char		*endl_ptr;

	endl_ptr = ft_strchr(*stored, '\n');
	if (endl_ptr == NULL)
	{
		tmp_ptr = ft_strdup(*stored);
		free(*stored);
		*stored = NULL;
		return (tmp_ptr);
	}
	ret = (char *)malloc(sizeof(char) * (endl_ptr - *stored + 1));
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, *stored, endl_ptr - *stored + 1);
	tmp_ptr = *stored;
	*stored = ft_strdup(++endl_ptr);
	if (*stored == NULL)
		return (NULL);
	free(tmp_ptr);
	return (ret);
}

/*
** A function for the case where there is no new line symbol
** in the string "stored" read from the file upto a new line symbol.
** return 1 if there's more to get.
** return 0 if EOF is reached.
*/

int	read_continue(int fd, char **stored)
{
	char		*buf;
	ssize_t		num_read;

	num_read = BUFFER_SIZE;
	buf = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buf)
		return (-1);
	while (ft_strchr(*stored, '\n') == NULL && num_read == BUFFER_SIZE)
	{
		num_read = read(fd, buf, BUFFER_SIZE);
		if (num_read < 0)
			break ;
		*stored = ft_strjoin_with_free(*stored, buf, num_read);
		if (*stored == NULL)
		{
			num_read = -1;
			break ;
		}
	}
	free(buf);
	if (num_read < 0)
		return (-1);
	if (ft_strchr(*stored, '\n') != NULL)
		return (1);
	return (0);
}

int	get_next_line(int fd, char **line)
{
	static char	*stored;
	int			flag_continue;

	flag_continue = 1;
	if (fd < 0 || !line || BUFFER_SIZE <= 0)
		return (-1);
	if (ft_strchr(stored, '\n') == NULL)
	{
		flag_continue = read_continue(fd, &stored);
		if (flag_continue == -1)
			return (-1);
	}
	if (flag_continue == 1)
	{
		*line = get_stored_line(&stored);
		if (*line == NULL)
			return (-1);
	}
	if (flag_continue == 0)
	{
		*line = get_stored_line(&stored);
		if (*line == NULL)
			return (-1);
	}
	return (flag_continue);
}

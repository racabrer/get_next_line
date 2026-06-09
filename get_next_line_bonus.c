/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raqcabre <raqcabre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 16:24:21 by raqcabre          #+#    #+#             */
/*   Updated: 2026/06/09 16:25:07 by raqcabre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*gnl_update_saved(char *saved)
{
	char	*new_from_left;
	char	*remain;
	size_t	i;

	new_from_left = ft_strchr(saved, '\n');
	if (!new_from_left)
		return (NULL);
	new_from_left++;
	if (!new_from_left)
		return (NULL);
	if (*new_from_left == '\0')
		return (NULL);
	remain = malloc(ft_strlen(new_from_left) + 1);
	if (!remain)
		return (NULL);
	i = 0;
	while (new_from_left[i] != '\0')
	{
		remain[i] = new_from_left[i];
		i++;
	}
	remain[i] = '\0';
	return (remain);
}

void	gnl_free_and_null(char **ptr)
{
	if (ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

char	*gnl_get_line_from_saved(char *saved)
{
	char	*new_line;
	size_t	len;
	size_t	i;

	if (!saved || *saved == '\0')
		return (NULL);
	len = 0;
	while (saved[len] && saved[len] != '\n')
		len++;
	if (saved[len] == '\n')
		len++;
	new_line = malloc(len + 1);
	if (!new_line)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_line[i] = saved[i];
		i++;
	}
	new_line[len] = '\0';
	return (new_line);
}

char	*gnl_read_and_join(char *saved, int fd)
{
	int		n;
	char	*temp;
	char	buf[BUFFER_SIZE + 1];

	n = 1;
	while (n > 0 && ft_strchr(saved, '\n') == NULL)
	{
		n = read(fd, buf, BUFFER_SIZE);
		if (n < 0)
		{
			free(saved);
			return (NULL);
		}
		if (n == 0)
			break ;
		buf[n] = '\0';
		temp = ft_strjoin(saved, buf);
		free(saved);
		saved = temp;
	}
	return (saved);
}

char	*get_next_line(int fd)
{
	static char	*saved[MAX_FD];
	char		*line;
	char		*remaining;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= MAX_FD)
		return (NULL);
	if (!saved[fd])
	{
		saved[fd] = malloc(1);
		if (!saved[fd])
			return (NULL);
		saved[fd][0] = '\0';
	}
	saved[fd] = gnl_read_and_join(saved[fd], fd);
	if (!saved[fd])
		return (NULL);
	line = gnl_get_line_from_saved(saved[fd]);
	if (!line)
		return (free(saved[fd]), (saved[fd] = NULL));
	remaining = gnl_update_saved(saved[fd]);
	free(saved[fd]);
	saved[fd] = remaining;
	return (line);
}

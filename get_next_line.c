/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raqcabre <raqcabre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 16:06:18 by raqcabre          #+#    #+#             */
/*   Updated: 2026/06/10 18:38:52 by raqcabre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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

void	*free_two_and_null(void *a, void *b)
{
	free(a);
	free(b);
	return (NULL);
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
	char	*buf;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	n = 1;
	while (n > 0 && ft_strchr(saved, '\n') == NULL)
	{
		n = read(fd, buf, BUFFER_SIZE);
		if (n < 0)
			return (free_two_and_null(buf, saved));
		if (n == 0)
			break ;
		buf[n] = '\0';
		temp = ft_strjoin(saved, buf);
		if (!temp)
			return (free_two_and_null(buf, saved));
		free(saved);
		saved = temp;
	}
	return (free(buf), saved);
}

char	*get_next_line(int fd)
{
	static char	*saved;
	char		*line;
	char		*remaining;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!saved)
	{
		saved = malloc(1);
		if (!saved)
			return (NULL);
		saved[0] = '\0';
	}
	saved = gnl_read_and_join(saved, fd);
	if (!saved)
		return (NULL);
	line = gnl_get_line_from_saved(saved);
	if (!line)
	{
		free(saved);
		return (saved = NULL, NULL);
	}
	remaining = gnl_update_saved(saved);
	free(saved);
	return (saved = remaining, line);
}

/*
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int	main(void)
{
	int fd;
	char *line;
	int line_count;

	fd = open("prueba.txt", O_RDONLY);
	if (fd == -1)
	{
		printf("Error al abrir el archivo\n");
		return (1);
	}

	line_count = 1;
	// Llamamos a tu función en un bucle hasta que devuelva NULL
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("Línea %d: %s", line_count, line);
		free(line);
		line_count++;
	}

	close(fd);
	return (0);
}*/
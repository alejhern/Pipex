/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 20:21:14 by alejhern          #+#    #+#             */
/*   Updated: 2024/10/02 02:23:14 by amhernandez      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_bufferjoin(char *s1, char *s2, int put_nl)
{
	size_t	t_size;
	size_t	s1_size;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (s1);
	s1_size = ft_strlen(s1);
	t_size = s1_size + ft_strlen(s2) + 1 + put_nl;
	s1 = (char *)ft_realloc(s1, t_size * sizeof(char));
	if (!s1)
		return (NULL);
	while (*s2)
		s1[s1_size++] = *s2++;
	if (put_nl)
		s1[s1_size++] = '\n';
	return (s1);
}

static char	**buffer_check_nl(char **buffer)
{
	buffer[2] = ft_strchr(buffer[1], '\n');
	if (buffer[2])
	{
		*buffer[2] = '\0';
		buffer[0] = ft_bufferjoin(buffer[0], buffer[1], 1);
		ft_memmove(buffer[1], buffer[2] + 1, ft_strlen(buffer[2] + 1) + 1);
	}
	else
	{
		buffer[0] = ft_bufferjoin(buffer[0], buffer[1], 0);
		*buffer[1] = '\0';
	}
	return (buffer);
}

static char	**get_new_buffer(int fd, char **buffer)
{
	int	bytes_read;

	buffer_check_nl(buffer);
	if (buffer[2] || !buffer[0])
		return (buffer);
	while (!buffer[2])
	{
		bytes_read = read(fd, buffer[1], BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(buffer[0]);
			buffer[0] = NULL;
			break ;
		}
		buffer[1][bytes_read] = '\0';
		if (bytes_read == 0)
			break ;
		buffer_check_nl(buffer);
	}
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer[1024][4] = {{NULL}};

	if (fd < 0 || fd > 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!buffer[fd][1])
	{
		buffer[fd][1] = (char *)ft_calloc((BUFFER_SIZE + 1), sizeof(char));
		if (!buffer[fd][1])
			return (NULL);
	}
	buffer[fd][0] = ft_strdup("");
	get_new_buffer(fd, buffer[fd]);
	if (buffer[fd][0] && buffer[fd][0][0] == '\0')
	{
		free(buffer[fd][0]);
		buffer[fd][0] = NULL;
	}
	if (!buffer[fd][0])
	{
		free(buffer[fd][1]);
		buffer[fd][1] = NULL;
	}
	return (buffer[fd][0]);
}

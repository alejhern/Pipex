/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 20:21:14 by alejhern          #+#    #+#             */
/*   Updated: 2024/08/09 11:12:45 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_buffer_join(char *s1, char *s2)
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
	t_size = s1_size + ft_strlen(s2) + 1;
	s1 = (char *)ft_realloc(s1, t_size * sizeof(char));
	if (!s1)
		return (NULL);
	while (*s2)
		s1[s1_size++] = *s2++;
	return (s1);
}

static char	**buffer_check_nl(char **buffer, unsigned int buffer_ch)
{
	buffer[3] = ft_strchr(buffer[buffer_ch], '\n');
	if (buffer[3])
	{
		*buffer[3] = '\0';
		buffer[0] = ft_buffer_join(buffer[0], buffer[buffer_ch]);
		buffer[0] = ft_buffer_join(buffer[0], "\n");
		buffer[3] = ft_strdup(buffer[3] + 1);
		free(buffer[2]);
		buffer[2] = ft_strdup(buffer[3]);
		free(buffer[3]);
		buffer[3] = NULL;
	}
	else
	{
		buffer[0] = ft_buffer_join(buffer[0], buffer[buffer_ch]);
		free(buffer[2]);
		buffer[2] = NULL;
	}
	return (buffer);
}

static char	**get_new_buffer(int fd, char **buffer)
{
	int	bytes_read;

	if (buffer[2])
	{
		buffer_check_nl(buffer, 2);
		if (buffer[2])
			return (buffer);
	}
	while (1)
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
		buffer_check_nl(buffer, 1);
		if (buffer[2])
			break ;
	}
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer[1024][4];

	if (fd < 0 || fd > 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer[fd][1] = (char *)ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!buffer[fd][1])
		return (NULL);
	buffer[fd][0] = NULL;
	get_new_buffer(fd, buffer[fd]);
	if (buffer[fd][0] && buffer[fd][0][0] == '\0')
	{
		free(buffer[fd][0]);
		buffer[fd][0] = NULL;
	}
	if (!buffer[fd][0])
	{
		free(buffer[fd][2]);
		buffer[fd][2] = NULL;
	}
	free(buffer[fd][1]);
	return (buffer[fd][0]);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 14:41:57 by alejhern          #+#    #+#             */
/*   Updated: 2024/07/06 03:36:44 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(char const *s, char c)
{
	int	counter;

	if (!s)
		return (0);
	counter = 0;
	while (*s)
	{
		if (*s != c)
			counter++;
		while (*s && *s != c)
		{
			s++;
			if (*s == '\0')
				return (counter);
		}
		s++;
	}
	return (counter);
}

static void	ft_free(char **farray, int failed)
{
	while (failed > 0)
	{
		free(farray[failed - 1]);
		failed--;
	}
	free(farray);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	int		num_words;
	int		start;
	int		word_len;

	array = (char **)ft_calloc(ft_count(s, c) + 1, sizeof(char *));
	if (!array)
		return (NULL);
	num_words = 0;
	start = 0;
	while (num_words < ft_count(s, c))
	{
		while (s[start] && s[start] == c)
			start++;
		word_len = 0;
		while (s[start + word_len] && s[start + word_len] != c)
			word_len++;
		array[num_words] = ft_substr(s, start, word_len);
		if (!array[num_words++])
			return (ft_free(array, num_words), NULL);
		start += word_len;
	}
	array[num_words] = NULL;
	return (array);
}

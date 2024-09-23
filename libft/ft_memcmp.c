/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 21:19:15 by alejhern          #+#    #+#             */
/*   Updated: 2024/07/02 22:55:59 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*arr1;
	unsigned char	*arr2;
	size_t			index;

	arr1 = (unsigned char *) s1;
	arr2 = (unsigned char *) s2;
	index = 0;
	if (n == 0)
		return (0);
	while (arr1[index] == arr2[index] && index < n - 1)
		index++;
	return (arr1[index] - arr2[index]);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:27:53 by alejhern          #+#    #+#             */
/*   Updated: 2024/09/09 03:40:50 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_countdigits_base(unsigned long long nb, int base)
{
	size_t	len;

	len = 1;
	while (nb / base != 0)
	{
		len++;
		nb /= base;
	}
	return (len);
}

char	*ft_itoa_base(unsigned long long nb, int base)
{
	size_t			digits;
	char			*str;

	if (base < 2 || base > 16)
		return (NULL);
	digits = ft_countdigits_base(nb, base);
	str = (char *)ft_calloc(digits + 1, sizeof(char));
	if (!str)
		return (NULL);
	while (digits--)
	{
		str[digits] = "0123456789abcdef"[nb % base];
		nb /= base;
	}
	return (str);
}

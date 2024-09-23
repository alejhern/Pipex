/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 20:57:16 by alejhern          #+#    #+#             */
/*   Updated: 2024/09/16 21:28:11 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_fd(int nbr, int fd)
{
	char	*nb;

	if (nbr == INT_MIN)
		return (ft_putstr_fd("-2147483648", fd));
	nb = ft_itoa(nbr);
	if (!nb)
		return (-1);
	nbr = ft_putstr_fd(nb, fd);
	free (nb);
	return (nbr);
}

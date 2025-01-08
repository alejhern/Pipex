/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 01:04:50 by amhernandez       #+#    #+#             */
/*   Updated: 2025/01/08 01:10:40 by amhernandez      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_array(void ***array)
{
	void	**ptr;

	if (!array || !*array)
		return ;
	ptr = *array;
	while (*ptr)
		free(*ptr++);
	free(*array);
	*array = NULL;
}

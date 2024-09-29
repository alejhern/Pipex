/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_parse_flags.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 21:34:28 by alejhern          #+#    #+#             */
/*   Updated: 2024/09/16 21:34:33 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

t_flags	init_flags(int bl_mayus)
{
	t_flags	flags;

	flags.minus = 0;
	flags.zero = 0;
	flags.width = -1;
	flags.dot = -1;
	flags.hash = 0;
	flags.space = 0;
	flags.plus = 0;
	flags.bl_mayus = bl_mayus;
	flags.prefix = NULL;
	return (flags);
}

static t_flags	parse_nbr_flags(t_flags flags, const char **str)
{
	while (**str == '.' || ft_isdigit(**str))
	{
		if (**str == '.')
		{
			(*str)++;
			if (ft_isdigit(**str))
				flags.dot = ft_atoi(*str);
			else
				flags.dot = 0;
			while (ft_isdigit(**str))
				(*str)++;
		}
		else if (ft_isdigit(**str))
		{
			flags.width = ft_atoi(*str);
			while (ft_isdigit(**str))
				(*str)++;
		}
		else
			(*str)++;
	}
	return (flags);
}

t_flags	parse_flags(const char **str)
{
	t_flags	flags;

	flags = init_flags(0);
	while (ft_strchr("0-# +", **str))
	{
		if (**str == '0' && flags.width == -1 && flags.minus == 0)
			flags.zero = 1;
		else if (**str == '-')
			flags.minus = 1;
		else if (**str == '#')
			flags.hash = 1;
		else if (**str == ' ')
			flags.space = 1;
		else if (**str == '+')
			flags.plus = 1;
		(*str)++;
	}
	flags = parse_nbr_flags(flags, str);
	return (flags);
}

void	ft_printpad(char pad_char, int pad_len, int *len)
{
	while (pad_len-- > 0)
		ft_printchar(pad_char, init_flags(0), len);
}

void	ft_printnbr_flags(int nb, t_flags flags, int total_len, int *len)
{
	if (flags.prefix)
	{
		if (flags.zero && total_len > 0 && flags.dot == -1)
		{
			ft_printstr(flags.prefix, init_flags(flags.bl_mayus), len);
			ft_printpad('0', total_len, len);
		}
		else
		{
			ft_printpad(' ', total_len, len);
			ft_printstr(flags.prefix, init_flags(flags.bl_mayus), len);
		}
		return ;
	}
	if (nb < 0 || flags.plus || flags.space)
		total_len++;
	if (nb < 0)
		ft_printchar('-', init_flags(flags.bl_mayus), len);
	else if (flags.plus)
		ft_printchar('+', init_flags(flags.bl_mayus), len);
	else if (flags.space)
		ft_printchar(' ', init_flags(flags.bl_mayus), len);
	if (!flags.minus && flags.zero && flags.dot == -1)
		ft_printpad('0', flags.width - total_len, len);
}

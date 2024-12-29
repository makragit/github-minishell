/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sign_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:27:14 by mkrausho          #+#    #+#             */
/*   Updated: 2024/05/15 18:27:15 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	isflag(char c);

t_fptr	sign_handler(const char *format, int *i, int *ret)
{
	int	(*f_ptr)(void *);
	int	j;

	j = (*i)++;
	while (isflag(format[*i]) && *i < ft_strlen(format))
		(*i)++;
	if (*i >= ft_strlen(format))
	{
		*ret = -1;
		return (NULL);
	}
	f_ptr = ret_fp(format[*i]);
	if (f_ptr == &fp_psign)
	{
		ft_putchar_fd('%', 1);
		(*ret)++;
		return (f_ptr);
	}
	if (f_ptr)
		return (f_ptr);
	ft_putchar_fd('%', 1);
	*i = j;
	(*ret)++;
	return (NULL);
}

static int	isflag(char c)
{
	if (c == '-')
		return (1);
	if (c == '+')
		return (1);
	if (c == '0')
		return (1);
	if (c == '#')
		return (1);
	if (c == ' ')
		return (1);
	return (0);
}

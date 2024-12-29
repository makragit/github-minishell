/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:15:53 by mkrausho          #+#    #+#             */
/*   Updated: 2024/05/15 18:18:11 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_putchar_fd_ret(char c, int fd);

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		(*f_ptr)(void *);
	int		i;
	int		ret;

	if (format == NULL)
		return (-1);
	ret = 0;
	va_start(args, format);
	i = 0;
	while (i < ft_strlen(format) && ret != -1)
	{
		if (format[i] == '%')
		{
			f_ptr = sign_handler(format, &i, &ret);
			if (f_ptr != NULL && f_ptr != &fp_psign)
				ret += (f_ptr)(va_arg(args, void *));
		}
		else
			ret += ft_putchar_fd_ret(format[i], 1);
		i++;
	}
	va_end (args);
	return (ret);
}

static int	ft_putchar_fd_ret(char c, int fd)
{
	write(fd, &c, 1);
	return (1);
}

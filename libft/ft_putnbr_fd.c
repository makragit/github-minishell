/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:31:34 by mkrausho          #+#    #+#             */
/*   Updated: 2024/04/23 22:54:14 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Name					ft_putnbr_fd
Prototype			void ft_putnbr_fd(int n, int fd);
Parameters		n: The integer to output.
							fd: The file descriptor on which to write.
Return				None
external			write
Description		Outputs the integer ’n’ to the given file
							descriptor.
*/

//LLONG_MIN      = -9223372036854775808 20
//LLONG_MAX      = 9223372036854775807

#include <unistd.h>
#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	signed long long int	nl;

	nl = n;
	if (nl < 0)
	{
		if (nl == (1LL << 63))
		{
			write(fd, "-9223372036854775808", 20);
			return ;
		}
		ft_putchar_fd('-', fd);
		nl = nl * (-1);
	}
	if (nl > 9)
	{
		ft_putnbr_fd(nl / 10, fd);
	}
	ft_putchar_fd(nl % 10 + '0', fd);
}

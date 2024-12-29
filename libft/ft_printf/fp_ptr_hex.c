/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fp_ptr_hex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:19:11 by mkrausho          #+#    #+#             */
/*   Updated: 2024/05/15 21:38:56 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	fp_ptr_hex(void *c)
{
	unsigned long int	n;
	char				*base;
	int					digits;

	if (!c)
	{
		ft_putstr_fd("(nil)", 1);
		return (5);
	}
	base = ft_strdup("0123456789abcdef");
	ft_putstr_fd("0x", 1);
	n = *(unsigned long int *)&c;
	fp_hex_putnbr (n, base);
	free(base);
	digits = 2;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n /= 16;
		digits++;
	}
	return (digits);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fp_upper_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:27:56 by mkrausho          #+#    #+#             */
/*   Updated: 2024/05/15 21:38:34 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	fp_upper_hex(void *c)
{
	unsigned int	n;
	int				digits;
	char			*base;

	base = ft_strdup("0123456789ABCDEF");
	n = *(unsigned int *)&c;
	fp_hex_putnbr(n, base);
	free(base);
	digits = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n /= 16;
		digits++;
	}
	return (digits);
}

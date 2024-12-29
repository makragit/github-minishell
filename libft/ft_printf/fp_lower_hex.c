/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fp_lower_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:23:53 by mkrausho          #+#    #+#             */
/*   Updated: 2024/05/15 21:38:15 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	fp_lower_hex(void *c)
{
	unsigned int	n;
	char			*base;
	int				digits;

	base = ft_strdup("0123456789abcdef");
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

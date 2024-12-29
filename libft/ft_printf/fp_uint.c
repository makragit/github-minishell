/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fp_uint.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:21:49 by mkrausho          #+#    #+#             */
/*   Updated: 2024/05/15 18:22:31 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	fp_uint(void *c)
{
	unsigned int	d;
	int				digits;

	d = *(unsigned int *)&c;
	fp_uint_putnbr(d);
	digits = 0;
	if (d == 0)
		return (1);
	while (d != 0)
	{
		d /= 10;
		digits++;
	}
	return (digits);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fp_int.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:26:06 by mkrausho          #+#    #+#             */
/*   Updated: 2024/05/15 18:26:07 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	fp_int(void *c)
{
	int	d;
	int	digits;

	digits = 0;
	d = *(int *)&c;
	ft_putnbr_fd(d, 1);
	if (d == 0)
		return (1);
	if (d < 0)
		digits++;
	while (d != 0)
	{
		d /= 10;
		digits++;
	}
	return (digits);
}

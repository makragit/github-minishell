/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fp_hex_putnbr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:26:35 by mkrausho          #+#    #+#             */
/*   Updated: 2024/05/15 18:26:35 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	fp_hex_putnbr(unsigned long int n, char *base)
{
	if (!base)
		return ;
	if (n > 15)
	{
		fp_hex_putnbr(n / 16, base);
	}
	ft_putchar_fd(base[n % 16], 1);
}

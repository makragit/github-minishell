/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 11:44:28 by mkrausho          #+#    #+#             */
/*   Updated: 2024/10/16 19:41:04 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Function name   ft_itoa
Prototype       char *ft_itoa(int n);
Parameters      n: the integer to convert.
                The string representing the integer.
Return value    NULL if the allocation fails.
External fun    malloc
                Allocates (with malloc(3)) and returns a string
Description     representing the integer received as an argument.
                Negative numbers must be handled.
*/

#include "libft.h"
#include <stdlib.h>
#include <stddef.h>

static char	*itoa_return(int n, int len, int sign);

char	*ft_itoa(int n)
{
	int		sign;
	int		len;
	int		temp;

	if (n == 0)
		return (ft_strdup("0"));
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	sign = 0;
	if (n < 0)
	{
		sign = 1;
		n = n * -1;
	}
	temp = n;
	len = 0;
	while (temp)
	{
		temp = temp / 10;
		len++;
	}
	return (itoa_return(n, len, sign));
}

static char	*itoa_return(int n, int len, int sign)
{
	char	*new;

	new = (char *)malloc(sizeof(char) * (len + 1 + sign));
	if (!new)
		return (NULL);
	new[0] = '-';
	new[len + sign] = '\0';
	while (len--)
	{
		new[len + sign] = (n % 10) + '0';
		n /= 10;
	}
	return (new);
}

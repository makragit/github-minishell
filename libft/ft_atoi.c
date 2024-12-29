/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 18:56:20 by mkrausho          #+#    #+#             */
/*   Updated: 2024/04/24 17:12:41 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// loop spaces, check if single +/-, loop digits

static int	ft_isspace(char c);

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sum;
	int	sign;

	sum = 0;
	sign = 1;
	i = 0;
	while (ft_isspace(nptr[i]))
		i++;
	if (nptr[i] == '-')
	{
		sign = sign * (-1);
		i++;
	}
	else if (nptr[i] == '+')
	{
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		sum = sum * 10 + nptr[i] - '0';
		i++;
	}
	return (sign * sum);
}

static int	ft_isspace(char c)
{
	if (c == ' ')
		return (1);
	if (c == '\f')
		return (1);
	if (c == '\n')
		return (1);
	if (c == '\r')
		return (1);
	if (c == '\t')
		return (1);
	if (c == '\v')
		return (1);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 19:53:13 by mkrausho          #+#    #+#             */
/*   Updated: 2024/04/19 19:46:07 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Name:           ft_striteri
Prototype:      void ft_striteri(char *s, void (*f)(unsigned int, char*));
Parameters:     s: The string on which to iterate.
                f: The function to apply to each character.
Return          None
External        None
Description     Applies the function ’f’ on each character of
                the string passed as argument, passing its index
                as first argument. Each character is passed by
                address to ’f’ to be modified if necessary.
*/

#include <stdlib.h>
#include <stddef.h>
#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	if (!s || !(*f) || !f)
		return ;
	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}

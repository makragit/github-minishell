/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 17:44:51 by mkrausho          #+#    #+#             */
/*   Updated: 2024/04/19 19:50:18 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Name:           ft_strmapi
Prototype:      char *ft_strmapi(char const *s, char (*f)(unsigned
                int, char));
Parameters:     s: The string on which to iterate.
                f: The function to apply to each character.
Return          The string created from the successive applications
                of ’f’.
								Returns NULL if the allocation fails.
External        malloc
Description:    Applies the function ’f’ to each character of the
                string ’s’, and passing its index as first argument
                to create a new string (with malloc(3)) resulting
                from successive applications of ’f’
*/

#include <stdlib.h>
#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*new;

	if (!s || !(*f) || !f)
		return (NULL);
	new = (char *)malloc((sizeof(char) * ft_strlen(s)) + 1);
	if (!new)
		return (NULL);
	i = 0;
	while (s[i])
	{
		new[i] = (*f)(i, s[i]);
		i++;
	}
	new[i] = '\0';
	return (new);
}

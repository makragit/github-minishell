/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 21:24:54 by mkrausho          #+#    #+#             */
/*   Updated: 2024/04/24 18:07:31 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Function name:  ft_strjoin
Prototype: char *ft_strjoin(char const *s1, char const *s2);

Parameters:     s1: The prefix string.
                s2: The suffix string.
Return value    The new string.
                NULL if the allocation fails.
extern          malloc
Description     Allocates (with malloc(3)) and returns a new
                string, which is the result of the concatenation
                of ’s1’ and ’s2’.
*/

#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		count;
	char	*new;
	size_t	len;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup((char *)s2));
	if (!s2)
		return (ft_strdup((char *)s1));
	len = ft_strlen(s1) + ft_strlen(s2);
	new = (char *)malloc(sizeof(char) * (len + 1));
	if (!new)
		return (NULL);
	count = 0;
	while (*s1)
		new[count++] = *s1++;
	while (*s2)
		new[count++] = *s2++;
	new[count] = '\0';
	return (new);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 22:35:54 by mkrausho          #+#    #+#             */
/*   Updated: 2024/04/23 16:13:34 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
function name   ft_strtrim
prototype       char *ft_strtrim(char const *s1, char const *set);
parameters      s1: The string to be trimmed.
                set: The reference set of characters to trim.
return          The trimmed string.
								NULL if the allocation fails.
external        malloc
Description     Allocates (with malloc(3)) and returns a copy of
                ’s1’ with the characters specified in ’set’ removed
                from the beginning and the end of the string.
*/

// count len and left in one loop; left stops at first non-set (left != len)
// reduce len for set-values on the right side
// use ft_substring to return trimmed string

#include <stddef.h>
#include "libft.h"

static int	in_set(char c, char const *set);

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	left;
	size_t	len;

	if (!s1)
		return (NULL);
	if (!set)
		return (ft_strdup((char *)s1));
	left = 0;
	len = 0;
	while (s1[len])
	{
		if (left == len)
			if (in_set(s1[len], set))
				left++;
		len++;
	}
	while (len > left)
	{
		if (in_set(s1[len - 1], set) == 0)
			break ;
		len--;
	}
	return (ft_substr(s1, left, (len - left)));
}

static int	in_set(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:42:07 by mkrausho          #+#    #+#             */
/*   Updated: 2024/04/21 21:16:37 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Function name:  ft_substr
                char *ft_substr(char const *s, unsigned int start, size_t len);
Turn in files:  -
Parameter:      s: The string from which to create the substring.
                start: The start index of the substring in the string ’s’.
                len: The maximum length of the substring. 
Return value:   The substring.
                NULL if the allocation fails.
External func:  malloc
Description:    Allocates (with malloc(3)) and returns a substring
                from the string ’s’.
                The substring begins at index ’start’ and is of
                maximum size ’len’.
*/

#include "libft.h"
#include <stdlib.h>
#include <stddef.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	len_s;
	size_t	len_m;
	size_t	i;

	if (!s)
		return (NULL);
	len_s = ft_strlen(s);
	if (start > (unsigned int)len_s)
		return (ft_strdup(""));
	len_m = len;
	if (len > (len_s - start))
		len_m = len_s - start;
	sub = (char *)malloc(sizeof(char) * (len_m + 1));
	if (!sub)
		return (NULL);
	while (start--)
		s++;
	i = 0;
	while (i < len && *s != 0)
		sub[i++] = *s++;
	sub[i] = '\0';
	return (sub);
}

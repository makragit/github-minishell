/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:27:35 by mkrausho          #+#    #+#             */
/*   Updated: 2024/04/21 21:18:18 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*s_p;

	s_p = (char *)s;
	while (*s != 0)
	{
		if (*s == (char)c)
			s_p = (char *)s;
		s++;
	}
	if (*s == (char)c)
		s_p = (char *)s;
	if (*s_p == (char)c)
		return ((char *)s_p);
	return (0);
}

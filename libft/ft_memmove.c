/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 19:47:52 by mkrausho          #+#    #+#             */
/*   Updated: 2024/04/21 21:21:23 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
DESCRIPTION
       The  memmove()  function  copies n bytes from memory area src to memory
       area dest.  The memory areas may overlap: copying takes place as though
       the  bytes in src are first copied into a temporary array that does not
       overlap src or dest, and the bytes are then copied from  the  temporary
       array to dest.

RETURN VALUE
       The memmove() function returns a pointer to dest.
*/

// if src > dest; n=1000	// _p[n] out of bounds, start from _p[0]!
// if src < dest; n=1000	// _p[n] '\0' will be at _p[-1], string will work

#include <stddef.h>

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t		i;
	void		*dest_v;
	char		*dest_p;
	const char	*src_p;

	dest_p = dest;
	src_p = src;
	dest_v = dest;
	/* if (!src && !dest) */
	if (!src || !dest) // Fix?
		return (dest);
	i = 0;
	if (dest < src)
	{
		while (i < n)
		{
			dest_p[i] = src_p[i];
			i++;
		}
	}
	if (dest > src)
	{
		while (n--)
			dest_p[n] = src_p[n];
	}
	return (dest_v);
}

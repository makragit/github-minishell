/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 21:14:06 by mkrausho          #+#    #+#             */
/*   Updated: 2024/04/21 21:15:01 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
DESCRIPTION
     The strlcpy() and strlcat() functions copy and concatenate strings re‐
     spectively.  They are designed to be safer, more consistent, and less er‐
     ror prone replacements for strncpy(3) and strncat(3).  Unlike those func‐
     tions, strlcpy() and strlcat() take the full size of the buffer (not just
     the length) and guarantee to NUL-terminate the result (as long as size is
     larger than 0 or, in the case of strlcat(), as long as there is at least
     one byte free in dst).  Note that a byte for the NUL should be included
     in size.  Also note that strlcpy() and strlcat() only operate on true “C”
     strings.  This means that for strlcpy() src must be NUL-terminated and
     for strlcat() both src and dst must be NUL-terminated.

     The strlcpy() function copies up to size - 1 characters from the NUL-ter‐
     minated string src to dst, NUL-terminating the result.

     The strlcat() function appends the NUL-terminated string src to the end
     of dst.  It will append at most size - strlen(dst) - 1 bytes, NUL-termi‐
     nating the result.

RETURN VALUES
     The strlcpy() and strlcat() functions return the total length of the
     string they tried to create.  For strlcpy() that means the length of src.
     For strlcat() that means the initial length of dst plus the length of
     src.  While this may seem somewhat confusing, it was done to make trunca‐
     tion detection simple.

     Note, however, that if strlcat() traverses size characters without find‐
     ing a NUL, the length of the string is considered to be size and the des‐
     tination string will not be NUL-terminated (since there was no space for
     the NUL).  This keeps strlcat() from running off the end of a string.  In
     practice this should not happen (as it means that either size is incor‐
     rect or that dst is not a proper “C” string).  The check exists to pre‐
     vent potential security problems in incorrect code.
*/

// rem 0 dst stays the same, rem 1 dst with 0 stays same
// src_p++ and dest_p++ until '\0', but dest_p stops when no writes remain.

#include <stddef.h>
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	char		*dest_p;
	size_t		left;
	const char	*src_p;

	if (size == 0)
		return (ft_strlen(src));
	dest_p = dst;
	src_p = src;
	while (size-- && *dest_p)
		dest_p++;
	left = (dest_p - dst);
	if (++size == 0 || size == 1)
		return (left + ft_strlen(src));
	while (*src_p)
	{
		if (size > 1)
		{
			*dest_p++ = *src_p;
			size--;
		}
		src_p++;
	}
	*dest_p = '\0';
	return (left + (src_p - src));
}

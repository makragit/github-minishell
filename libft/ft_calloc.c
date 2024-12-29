/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 15:18:44 by mkrausho          #+#    #+#             */
/*   Updated: 2024/04/23 15:46:19 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
The  calloc() function allocates memory for an array of nmemb
    elements of size bytes each and returns a pointer to the  al‐
	located memory.  The memory is set to zero.


If nmemb or size
    is 0, then calloc() returns either NULL, or a unique  pointer
	value  that  can  later be successfully passed to free().  If
	the multiplication of nmemb and size would result in  integer
	overflow,  then  calloc()  returns an error.


ERRORS
    calloc(), malloc(), realloc(), and  reallocarray()  can  fail
    with the following error:

    ENOMEM Out  of  memory.   Possibly,  the  application hit the
    RLIMIT_AS or  RLIMIT_DATA  limit  described  in  getr‐
    limit(2).

RETURN VALUE
    The  malloc()  and calloc() functions return a pointer to the
    allocated memory, which is suitably aligned for any  built-in
    type.   On error, these functions return NULL.  NULL may also
    be returned by a successful call to malloc() with a  size  of
    zero,  or by a successful call to calloc() with nmemb or size
    equal to zero
*/

#include <stdlib.h>
#include <stddef.h>
#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char		*new;	

	if ((nmemb * size) > 2147483647)
		return (NULL);
	new = (void *)malloc((nmemb * size));
	if (!new)
		return (NULL);
	ft_bzero(new, (nmemb * size));
	return (new);
}

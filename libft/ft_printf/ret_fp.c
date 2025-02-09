/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ret_fp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:29:08 by mkrausho          #+#    #+#             */
/*   Updated: 2024/05/15 18:29:32 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_fptr	ret_fp(const char c)
{
	int	(*fp)(void *);

	fp = NULL;
	if (c == 'c')
		return (&fp_char);
	if (c == 's')
		return (&fp_string);
	if (c == 'd')
		return (&fp_int);
	if (c == 'i')
		return (&fp_int);
	if (c == 'u')
		return (&fp_uint);
	if (c == 'x')
		return (&fp_lower_hex);
	if (c == 'X')
		return (&fp_upper_hex);
	if (c == 'p')
		return (&fp_ptr_hex);
	if (c == '%')
		return (&fp_psign);
	return (fp);
}

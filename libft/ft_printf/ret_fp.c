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
		return (fp = &fp_char);
	if (c == 's')
		return (fp = &fp_string);
	if (c == 'd')
		return (fp = &fp_int);
	if (c == 'i')
		return (fp = &fp_int);
	if (c == 'u')
		return (fp = &fp_uint);
	if (c == 'x')
		return (fp = &fp_lower_hex);
	if (c == 'X')
		return (fp = &fp_upper_hex);
	if (c == 'p')
		return (fp = &fp_ptr_hex);
	if (c == '%')
		return (fp = &fp_psign);
	return (fp);
}

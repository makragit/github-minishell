/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fp_string.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrausho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:21:16 by mkrausho          #+#    #+#             */
/*   Updated: 2024/05/15 18:21:32 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	fp_string(void *c)
{
	char	*str;

	if (c == NULL)
	{
		ft_putstr_fd("(null)", 1);
		return (6);
	}
	str = (char *)c;
	ft_putstr_fd(str, 1);
	return (ft_strlen(str));
}

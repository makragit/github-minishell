/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compress_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 09:34:15 by dbogovic          #+#    #+#             */
/*   Updated: 2025/02/10 09:34:34 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	compress_str(char *str, size_t str_size)
{
	size_t	src;
	size_t	dst;

	src = 0;
	dst = 0;
	while (src < str_size)
	{
		if (str[src] != '\0')
		{
			str[dst] = str[src];
			dst++;
		}
		src++;
	}
	str[dst] = '\0';
}

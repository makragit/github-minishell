/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip_s_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 14:15:47 by dbogovic          #+#    #+#             */
/*   Updated: 2025/02/01 14:17:37 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	skip_s_quote(char *str, int *c)
{
	if (str[*c] == '\'')
	{
		(*c)++;
		while (str[*c] && str[*c] != '\'')
			(*c)++;
		if (str[*c] == '\'')
			(*c)++;
	}
	if (!str[*c])
		return (-1);
	return (0);
}

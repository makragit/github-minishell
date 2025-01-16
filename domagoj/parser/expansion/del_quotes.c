/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_quotes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 20:32:28 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/16 19:04:45 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	ft_trim_quotes(char **str)
{
	char	*new;
	char	*strr;
	size_t	c;

	strr = *str;
	if (!*str || !str)
		return (-1);
	if (ft_strlen(*str) < 2)
		return (0);
	c = 0;
	while (strr[c])
	{
		if (strr[c] == '\'' || strr[c] == '\"')
		{
			if (strr[c] == '\"')
				new = ft_strexpel(*str, "\"", ALL);
			else if (strr[c] == '\'')
				new = ft_strexpel(*str, "\'", ALL);
			if (!new)
				return (-1);
			free(*str);
			*str = new;
			return (0);
		}
		c++;
	}
	return (0);
}

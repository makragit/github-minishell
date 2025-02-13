/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_quotes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 20:32:28 by domagoj           #+#    #+#             */
/*   Updated: 2025/02/10 11:58:51 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static int	remove_replace(char **str, char *rm)
{
	char	*tmp;

	tmp = ft_strexpel(*str, rm, ALL);
	if (!tmp)
		return (-1);
	free(*str);
	*str = tmp;
	return (0);
}

int	ft_trim_quotes(char **str)
{
	char	*strr;
	size_t	c;

	strr = *str;
	if (!*str || !str)
		return (-1);
	if (ft_strlen(*str) <= 2)
		return (0);
	c = 0;
	while (strr[c])
	{
		if (strr[c] == '\"')
			return (remove_replace(str, "\""));
		if (strr[c] == '\'')
			return (remove_replace(str, "\'"));
		c++;
	}
	return (0);
}

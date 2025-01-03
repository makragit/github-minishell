/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_quotes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 20:32:28 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/03 18:32:51 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	ft_trim_quotes(char **str)
{
	char	*strcpy;
	char	*new;
	size_t	len;

	strcpy = *str;
	if (!*str || !str)
		return (-1);
	len = ft_strlen(strcpy);
	if (len < 2)
		return (0);
	if (strcpy[0] == strcpy[len - 1])
	{
		if (strcpy[0] == '\'' || strcpy[0] == '\"')
		{
			strcpy[len - 1] = '\0';
			new = ft_strdup(strcpy + 1);
			if (!new)
				return (-1);
			free(*str);
			*str = new;
		}
	}
	return (0);
}

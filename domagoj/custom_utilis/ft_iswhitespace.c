/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_iswhitespace.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 22:10:55 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/03 18:29:47 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_whitespace(char c)
{
	if ((c > 8 && c < 14) || c == ' ')
		return (1);
	return (0);
}

int	skip_whitespace(char **str)
{
	size_t	c;
	char	*new;

	c = 0;
	while ((*str)[c] && is_whitespace((*str)[c]))
		c++;
	new = ft_strdup((*str) + c);
	if (!new)
		return (-1);
	free(*str);
	*str = new;
	return (0);
}

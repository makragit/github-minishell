/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_len.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 12:18:02 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/10 16:43:01 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static size_t	redir_len(char *str)
{
	size_t	len;

	len = ft_strlen(str);
	if (len <= 1)
		return (1);
	if (str[1] == '<' || str[1] == '>')
		return (2);
	return (1);
}

static size_t	word_len(char *str)
{
	size_t	c;

	c = 0;
	while (str[c])
	{
		if (str[c] == '|' || str[c] == 39 || str[c] == 34
			|| is_whitespace(str[c]))
			return (c);
		c++;
	}
	return (c);
}

static size_t	quote_len(char *str)
{
	size_t		c;

	c = 1;
	while (str[c] && str[c] != str[0])
		c++;
	return (c + 1);
}

size_t	token_len(char *str)
{
	if (str[0] == '|')
		return (1);
	if (str[0] == '\'' || str[0] == '\"')
		return (quote_len(str));
	if (str[0] == '<' || str[0] == '>')
		return (redir_len(str));
	return (word_len(str));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_len.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 12:18:02 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/29 17:06:06 by dbogovic         ###   ########.fr       */
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
	int		under_quote;

	c = 0;
	under_quote = 0;
	while (str[c])
	{
		if (str[c] == 39 || str[c] == 34)
		{
			if (under_quote == 0)
				under_quote = str[c];
			else
			{
				if (under_quote == str[c])
					under_quote = 0;
			}
		}
		if ((is_whitespace(str[c]) || str[c] == '|') && under_quote == 0)
			return (c);
		c++;
	}
	return (c);
}

size_t	token_len(char *str)
{
	if (str[0] == '|')
		return (1);
	if (str[0] == '<' || str[0] == '>')
		return (redir_len(str));
	return (word_len(str));
}

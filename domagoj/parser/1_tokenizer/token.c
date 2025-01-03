/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:23:28 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/03 18:21:41 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

t_token	*tokenize(char *input_string)
{
	t_token		*tokens;
	char		*str;
	char		*part1;
	char		*remainder;

	tokens = NULL;
	str = ft_strdup(input_string);
	if (!str)
		return (NULL);
	while (str)
	{
		if (skip_whitespace(&str) == -1)
		{
			free(str);
			fully_free(tokens);
			return (NULL);
		}
		if (str[0] == '\0')
			break ;
		part1 = ft_substr(str, 0, token_len(str));
		if (!part1)
		{
			free(str);
			fully_free(tokens);
			return (NULL);
		}
		remainder = ft_strdup(str + token_len(str));
		free(str);
		str = NULL;
		if (!remainder)
		{
			free(part1);
			fully_free(tokens);
			return (NULL);
		}
		str = remainder;
		if (make_token_entry(&tokens, part1) == -1)
		{
			free(part1);
			free(str);
			fully_free(tokens);
			return (NULL);
		}
	}
	tokens = reverse_lst(tokens);
	add_previous(tokens);
	free (str);
	token_categorisation(tokens);
	return (tokens);
}

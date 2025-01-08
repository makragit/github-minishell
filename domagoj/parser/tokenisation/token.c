/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:23:28 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/07 18:03:41 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static int	break_up_str(char **token_part, char **remainder_part)
{
	char	*tmp;

	if (!*remainder_part)
		return (1);
	*token_part = ft_substr(*remainder_part, 0, token_len(*remainder_part));
	if (!*token_part)
	{
		free(*remainder_part);
		*remainder_part = NULL;
		return (1);
	}
	tmp = ft_strdup(*remainder_part + token_len(*remainder_part));
	if (!tmp)
	{
		free(*token_part);
		free(*remainder_part);
		*remainder_part = NULL;
		return (1);
	}
	free(*remainder_part);
	*remainder_part = tmp;
	return (0);
}

t_token	*finish_process(t_token *tokens, char *str)
{
	if (!str)
	{
		free_lst(tokens);
		return (NULL);
	}
	tokens = reverse_lst(tokens);
	add_previous(tokens);
	free(str);
	token_categorisation(&tokens);
	return (tokens);
}

t_token	*tokenize(char *str)
{
	t_token		*tokens;
	char		*token_part;

	tokens = NULL;
	token_part = NULL;
	if (!str)
		return (NULL);
	while (str)
	{
		if (skip_whitespace(&str) == -1)
			break ;
		if (str[0] == '\0')
			break ;
		if (break_up_str(&token_part, &str))
			break ;
		if (make_token_entry(&tokens, token_part))
			break ;
	}
	return (finish_process(tokens, str));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:46:57 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/04 23:27:41 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static size_t	table_size(t_token *tokens)
{
	size_t	return_value;

	return_value = 1;
	while (tokens)
	{
		if (tokens->type == PIPE_TOKEN)
			return_value++;
		tokens = tokens->next;
	}
	return (return_value);
}

t_cmd_table	*parse(const char *input)
{
	t_token		*tokens;
	t_cmd_table	*cmd_table;

	cmd_table = NULL;
	if (input_check(input) == 1)
		return (NULL);
	tokens = tokenize(ft_strdup(input));
	if (!tokens)
		return (NULL);
	cmd_table = table_init(table_size(tokens));
	if (!cmd_table)
	{
		free_lst(tokens);
		return (NULL);
	}
	if (expander(tokens))
	{
		free_lst(tokens);
		tokens = NULL;
		free_table(cmd_table);
		return (NULL);
	}
	if (arr_create(cmd_table, tokens) == 1)
	{
		free_lst(tokens);
		tokens = NULL;
		free_table(cmd_table);
		return (NULL);
	}
	if (token_distribution(cmd_table, tokens))
	{
		free_lst(tokens);
		tokens = NULL;
		free_table(cmd_table);
		return (NULL);
	}
	return (cmd_table);
}

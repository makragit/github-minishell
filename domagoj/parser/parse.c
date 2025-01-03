/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:46:57 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/03 18:23:39 by mkrausho         ###   ########.fr       */
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
	char		*str;

	str = ft_strdup(input);
	if (!str)
		return (NULL);
	if (input_check(str) == 1)
	{
		free(str);
		return (NULL);
	}
	tokens = tokenize(str);
	free(str);
	if (!tokens)
		return (NULL);
	cmd_table = table_init(table_size(tokens));
	if (!cmd_table)
	{
		fully_free(tokens);
		return (NULL);
	}
	if (arr_create(cmd_table, tokens) == 1)
	{
		fully_free(tokens);
		free_table(cmd_table);
		return (NULL);
	}
	token_distribution(cmd_table, tokens);
	if (expander(&cmd_table) == 1)
	{
		free_table(cmd_table);
		return (NULL);
	}
	return (cmd_table);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:46:57 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/10 16:43:19 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	add_tokens_to_table(t_cmd_table *table, t_token *lst)
{
	while (table)
	{
		table->tokens = lst;
		table = table->next;
	}
}

static t_token	*parse_tokens(char *input)
{
	t_token	*tokens;

	tokens = tokenize(input);
	if (!tokens)
		return (NULL);
	if (expander(tokens) == ERROR)
	{
		free_lst(tokens);
		return (NULL);
	}
	return (tokens);
}

static t_cmd_table	*parse_table(t_token *tokens)
{
	t_cmd_table	*table;

	table = table_init((table_size(tokens)));
	if (!table)
		return (NULL);
	if (arr_create(table, tokens) == 1)
	{
		free_table(table);
		return (NULL);
	}
	if (token_distribution(table, tokens))
	{
		free_table(table);
		return (NULL);
	}
	return (table);
}

t_cmd_table	*parse(const char *input)
{
	char			*input_cpy;
	t_token			*token_lst;
	t_cmd_table		*table;

	input_cpy = ft_strdup(input);
	if (!input_cpy)
		return (NULL);
	if (input_check(input_cpy) == 1)
	{
		free(input_cpy);
		return (NULL);
	}
	token_lst = parse_tokens(input_cpy);
	if (!token_lst)
		return (NULL);
	table = parse_table(token_lst);
	if (!table)
	{
		free_lst(token_lst);
		return (NULL);
	}
	add_tokens_to_table(table, token_lst);
	return (table);
}

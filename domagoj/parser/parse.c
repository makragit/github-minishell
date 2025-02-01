/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:46:57 by domagoj           #+#    #+#             */
/*   Updated: 2025/02/01 17:01:21 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_token	*expel_bad(t_token *lst)
{
	t_token	*head;

	head = lst;
	while (lst)
	{
		if (lst->value[0] == '\0')
		{
			if (lst->type == CMD_t)
			{
				if (lst->next)
					lst->next->type = CMD_t;
			}
			if (lst == head)
				head = lst->next;
			free(lst->value);
			lst = cut_token(lst);
			continue ;
		}
		lst = lst->next;
	}
	return (head);
}

static t_token	*parse_tokens(char *input)
{
	t_token	*tokens;

	if (!input)
		return (NULL);
	tokens = tokenize(input);
	if (!tokens)
		return (NULL);
	if (expander(tokens) == ERROR)
	{
		free_lst(tokens);
		return (NULL);
	}
	tokens = expel_bad(tokens);
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

char	*trim_touching_quotes(char *str)
{
	char	*tmp;

	tmp = ft_strexpel(str, "\"\"", ALL);
	if (tmp)
	{
		free(str);
		str = tmp;
	}
	return (str);
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
	input_cpy = trim_touching_quotes(input_cpy);
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

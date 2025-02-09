/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: domagoj <domagoj@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:46:57 by domagoj           #+#    #+#             */
/*   Updated: 2025/02/06 16:39:36 by domagoj          ###   ########.fr       */
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

static void	compress_str(char *str, size_t str_size)
{
	size_t	src;
	size_t	dst;

	src = 0;
	dst = 0;
	while (src < str_size)
	{
		if (str[src] != '\0')
		{
			str[dst] = str[src];
			dst++;
		}
		src++;
	}
	str[dst] = '\0';
}

char	*trim_touching_quotes(char *str)
{
	size_t		str_size;
	size_t		i;
	size_t		start;
	char		q_flag;

	str_size = ft_strlen(str);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			q_flag = str[i];
			start = i;
			i++;
			while (str[i] && str[i] != q_flag)
				i++;
			if (str[i] == q_flag)
			{
				if (start + 1 == i)
				{
					str[start] = '\0';
					str[i] = '\0';
				}
			}
		}
		i++;
	}
	compress_str(str, str_size);
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
	expand_env(&input_cpy);
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

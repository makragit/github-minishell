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
static void compress_str(char *str, size_t str_size)
{
    size_t src = 0;
    size_t dst = 0;

    while (src < str_size)
    {
        if (str[src] != '\0')
        {
            str[dst] = str[src];
            dst++;
        }
        src++;
    }
    
    // Null-terminate the new compressed string
    str[dst] = '\0';
}


char *trim_touching_quotes(char *str)
{
    size_t str_size = ft_strlen(str);
    size_t i = 0;
    char q_flag;

    while (str[i])
    {
        if (str[i] == '\'' || str[i] == '\"')
        {
            q_flag = str[i]; // Store the quote type
            size_t start = i; // Store position of first quote
            i++;

            // Find closing quote
            while (str[i] && str[i] != q_flag)
                i++;

            if (str[i] == q_flag) // If properly closed
            {
				if (start + 1 == i)
				{
                	str[start] = '\0'; // Null out first quote
                	str[i] = '\0';     // Null out closing quote
				}
				// else if (start + 2 == i && (str[start + 1] == '\'' || str[start + 1] == '\"'))
				// {
				// 	str[start] = '\0'; // Null out first quote
                // 	str[i] = '\0';     // Null out closing quote
				// 	str[start + 1] = '\0';
				// }
			}
        }
        i++;
    }
	// Compress the string in place
    compress_str(str, str_size);

    return str;
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

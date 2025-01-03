/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 20:49:52 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/03 18:30:44 by mkrausho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

t_token	*cut_token(t_token *token)
{
	t_token	*previous;
	t_token	*next;

	if (token == NULL)
		return (NULL);
	previous = token->previous;
	next = token->next;
	if (previous)
		previous->next = next;
	if (next)
		next->previous = previous;
	free(token);
	token = NULL;
	return (next);
}

t_token	*fill_redir(t_cmd_table *table, t_token *token)
{
	if (token->type == APPEND)
	{
		if (table->append_redir)
			free(table->append_redir);
		table->append_redir = token->value;
	}
	else if (token->type == INPUT)
	{
		if (table->input_redir)
			free(table->input_redir);
		table->input_redir = token->value;
	}
	else if (token->type == OUTPUT)
	{
		if (table->output_redir)
			free(table->output_redir);
		table->output_redir = token->value;
	}
	else
	{
		if (table->heredoc_delim)
			free(table->heredoc_delim);
		table->heredoc_delim = token->value;
	}
	return (cut_token(token));
}

void	token_distribution(t_cmd_table *table, t_token *token)
{
	token_type	type;
	char *path_array[] = {"/usr/local/sbin", "/usr/local/bin", "/usr/sbin", "/usr/sbin", NULL};

	while (table)
	{
		if (!token)
			break ;
		type = token->type;
		if (type == CMD_TOKEN)
		{
			table->cmd_path = return_executable_path(token->value, path_array);
			token = cut_token(token);
		}
		else if (type == ARG_TOKEN)
			token = cut_token(token);
		else if (type == PIPE_TOKEN)
		{
			table = table->next;
			free(token->value);
			token = cut_token(token);
		}
		else if (type == HEREDOC || type == APPEND
			|| type == INPUT || type == OUTPUT)
			token = fill_redir(table, token);
		else
			table = table->next;
	}
}

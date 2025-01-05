/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 20:49:52 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/05 00:37:44 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

t_token	*cut_token(t_token *token)
{
	t_token	*previous;
	t_token	*next;

	if (token == NULL)
		return (NULL);
	previous = token->prev;
	next = token->next;
	if (previous)
		previous->next = next;
	if (next)
		next->prev = previous;
	free(token);
	token = NULL;
	return (next);
}

t_redir_data	*reverse_data_lst(t_redir_data *data)
{
	t_redir_data	*prev;
	t_redir_data	*current;
	t_redir_data	*next;

	prev = NULL;
	next = NULL;
	current = data;
	while (current != NULL)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	data = prev;
	return (data);
}

void	add_data_previous(t_redir_data *lst)
{
	t_redir_data	*tmp;

	tmp = NULL;
	while (lst)
	{
		lst->prev = tmp;
		tmp = lst;
		lst = lst->next;
	}
}

int	token_distribution(t_cmd_table *table, t_token *tokens)
{
	t_token_type	type;
	t_token			*token;

	token = tokens;
	while (table)
	{
		while (token)
		{
			type = token->type;
			if (type == PIPE_TOKEN)
			{
				printf("THERE IS A PIPE THERE!\n");
				token = token->next;
				break ;
			}
			else if (type == CMD_t)
				table->cmd = token->value;
			else if (type == HEREDOC || type == APPEND || type == INPUT || type == OUTPUT)
			{
				table->redir_data = add_redir_entry(table->redir_data, token);
				if (!table->redir_data)
					return (1);
			}
			token = token->next;
		}
		if (table->redir_data)
		{
			table->redir_data = reverse_data_lst(table->redir_data);
			add_data_previous(table->redir_data);
		}
		table->tokens = tokens;
		table = table->next;
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 20:49:52 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/22 14:14:17 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	fill_table(t_token *token, t_cmd_table *table)
{
	t_token_type	type;

	type = token->type;
	if (type == PIPE)
		return (2);
	else if (type == CMD_t)
	{
		table->cmd = token->value;
		return (0);
	}
	else if (type == HEREDOC || type == APPEND
		|| type == INPUT || type == OUTPUT)
	{
		table->redir_data = add_redir_entry(table->redir_data, token);
		if (!table->redir_data)
			return (1);
	}
	return (0);
}

int	token_distribution(t_cmd_table *table, t_token *tokens)
{
	int				result;
	t_token			*token;

	token = tokens;
	while (table)
	{
		while (token)
		{
			result = fill_table(token, table);
			if (result == 1)
				return (1);
			token = token->next;
			if (result == 2)
				break ;
		}
		if (table->redir_data)
		{
			table->redir_data = reverse_data_lst(table->redir_data);
			add_data_previous(table->redir_data);
		}
		table = table->next;
	}
	return (0);
}

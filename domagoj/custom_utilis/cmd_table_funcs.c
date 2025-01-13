/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table_funcs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 14:13:30 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/10 14:15:37 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

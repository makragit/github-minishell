/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:59:59 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/04 22:52:26 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_token	*reverse_lst(t_token *lst)
{
	t_token	*prev;
	t_token	*current;
	t_token	*next;

	prev = NULL;
	next = NULL;
	current = lst;
	while (current != NULL)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	lst = prev;
	return (lst);
}

void	add_previous(t_token *lst)
{
	t_token	*tmp;

	tmp = NULL;
	while (lst)
	{
		lst->prev = tmp;
		tmp = lst;
		lst = lst->next;
	}
}

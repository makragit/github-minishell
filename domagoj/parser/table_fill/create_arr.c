/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_arr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 22:50:12 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/22 14:12:21 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static size_t	arr_size(t_token *token)
{
	size_t	i;

	i = 0;
	while (token && token->type != CMD_t)
	{
		if (token->type == PIPE)
			return (0);
		token = token->next;
	}
	if (!token)
		return (0);
	while (token)
	{
		if (token->type != CMD_t && token->type != ARG_TOKEN)
			break ;
		i++;
		token = token->next;
	}
	return (i);
}

static t_token	*fill_array(char **arr, t_token *token, size_t size)
{
	size_t	i;

	i = 0;
	while (token && token->type != CMD_t)
		token = token->next;
	arr[i] = token->value;
	i++;
	token = token->next;
	while (i < size)
	{
		while (token->type != ARG_TOKEN)
			token = token->next;
		arr[i] = token->value;
		token = token->next;
		i++;
	}
	return (token);
}

int	arr_create(t_cmd_table *table, t_token *token)
{
	size_t		size;

	while (table)
	{
		if (token->type == PIPE)
			token = token->next;
		size = arr_size(token);
		if (!size)
		{
			while (token && token->type != CMD_t)
				token = token->next;
			table->args = NULL;
			table = table->next;
			continue ;
		}
		table->args = malloc(sizeof(char *) * (size + 1));
		if (!(table->args))
			return (1);
		(table->args)[size] = NULL;
		token = fill_array(table->args, token, size);
		table = table->next;
	}
	return (0);
}

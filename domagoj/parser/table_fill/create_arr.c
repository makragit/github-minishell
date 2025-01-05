/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_arr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 22:50:12 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/04 22:53:08 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static size_t	arr_size(t_token *token)
{
	size_t	i;

	i = 1;
	if (!token)
		return (0);
	while (token)
	{
		if (token->type == CMD_t)
		{
			token = token->next;
			break ;
		}
		token = token->next;
	}
	while (token)
	{
		if (token->type == PIPE_TOKEN)
			break ;
		if (token->type == ARG_TOKEN)
			i++;
		token = token->next;
	}
	return (i);
}

static t_token	*fill_array(char **arr, t_token *token, size_t size)
{
	size_t	i;

	i = 0;
	while (token->type != CMD_t)
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
		size = arr_size(token);
		if (!size)
			return (1);
		table->args = malloc(sizeof(char *) * (size + 1));
		if (!(table->args))
			return (1);
		(table->args)[size] = NULL;
		token = fill_array(table->args, token, size);
		table = table->next;
	}
	return (0);
}

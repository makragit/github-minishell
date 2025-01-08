/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 15:48:56 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/07 20:20:04 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

size_t	table_size(t_token *tokens)
{
	size_t	return_value;

	return_value = 1;
	while (tokens)
	{
		if (tokens->type == PIPE_TOKEN)
			return_value++;
		tokens = tokens->next;
	}
	return (return_value);
}

static void	fill_basic(t_cmd_table *new, size_t index)
{
	ft_memset(new, 0, sizeof(t_cmd_table));
	new->p_input_fd = -1;
	new->p_output_fd = -1;
	new->heredoc_fd = -1;
	new->index = index;
}

t_cmd_table	*table_init(size_t of_size)
{
	size_t		i;
	t_cmd_table	*new;
	t_cmd_table	*table;

	if (of_size == 0)
		return (NULL);
	i = 0;
	table = NULL;
	while (i < of_size)
	{
		new = malloc(sizeof(t_cmd_table));
		if (!new)
		{
			if (table)
				free_table(table);
			return (NULL);
		}
		fill_basic(new, of_size - i);
		new->next = table;
		table = new;
		i++;
	}
	return (table);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 15:48:56 by domagoj           #+#    #+#             */
/*   Updated: 2025/01/05 19:21:30 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

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
		ft_memset(new, 0, sizeof(t_cmd_table));
		new->p_input_fd = -1;
		new->p_output_fd = -1;
		new->heredoc_fd = -1;
		new->index = of_size - i;
		new->next = table;
		table = new;
		i++;
	}
	return (table);
}

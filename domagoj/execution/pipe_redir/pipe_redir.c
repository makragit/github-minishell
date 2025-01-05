/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:10:35 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/05 19:06:18 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	pipe_setup(t_cmd_table *table)
{
	int fd[2];
	if (!table->next)
		return (SINGLE_CMD);
	while (table)
	{
		if (pipe(fd) == -1)
			return (-1);
		table->p_output_fd = fd[1];
		table = table->next;
		if (table)
			table->p_input_fd = fd[0];
	}
	return (OK);
}

int close_pipes(t_cmd_table *table)
{
	while (table)
	{
		if (table->p_input_fd != -1)
			close(table->p_input_fd);
		if (table->p_output_fd != -1)
			close(table->p_input_fd);
		table = table->next;
	}
	return (0);
}

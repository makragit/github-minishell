/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:10:35 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/07 20:12:21 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	pipe_setup(t_cmd_table *table)
{
	t_cmd_table		*current;
	int				pipefd[2];

	current = table;
	while (current && current->next)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			return (FAIL);
		}
		current->p_output_fd = pipefd[1];
		current = current->next;
		current->p_input_fd = pipefd[0];
	}
	return (OK);
}

int	close_pipes(t_cmd_table *table)
{
	if (table)
	{
		if (table->p_input_fd != -1)
		{
			close(table->p_input_fd);
			table->p_input_fd = -1;
		}
		if (table->p_output_fd != -1)
		{
			close(table->p_output_fd);
			table->p_output_fd = -1;
		}
	}
	return (0);
}
/*
pipe redir how it works:
write end of the pipe (fd[1]) IS FD WHERE YOU MUST OUTPUT TO THE PIPE

read end of the pipe (fd[0]) IS WHERE STDIN IS for next command

*/

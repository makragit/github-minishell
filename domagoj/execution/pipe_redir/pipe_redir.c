/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:10:35 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/29 16:58:44 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	all_pipes_init(t_cmd_table *table)
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

int	pipe_redir(t_cmd_table *table)
{
	if (table->p_input_fd != -1)
	{
		dup2(table->p_input_fd, STDIN_FILENO);
		close(table->p_input_fd);
		table->p_input_fd = -1;
	}
	if (table->p_output_fd != -1)
	{
		dup2(table->p_output_fd, STDOUT_FILENO);
		close(table->p_output_fd);
		table->p_output_fd = -1;
	}
	return (0);
}

int	close_unused_pipes(t_cmd_table *head)
{
	while (head)
	{
		if (head->p_input_fd != -1)
		{
			close(head->p_input_fd);
			head->p_input_fd = -1;
		}
		if (head->p_output_fd != -1)
		{
			close(head->p_output_fd);
			head->p_output_fd = -1;
		}
		head = head->next;
	}
	return (0);
}

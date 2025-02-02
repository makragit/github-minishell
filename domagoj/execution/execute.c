/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:12:41 by dbogovic          #+#    #+#             */
/*   Updated: 2025/02/02 14:59:59 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_err	single_builtin_line(t_cmd_table *table)
{
	if (!table->next && is_builtin(table->cmd))
	{
		get_data(NULL)->backup_in = dup(STDIN_FILENO);
		get_data(NULL)->backup_out = dup(STDOUT_FILENO);
		if (redir(table->redir_data, table->redir_data) == OK)
		{
			get_data(NULL)->last_ex_code = try_builtin(table);
		}
		dup2(get_data(NULL)->backup_in, STDIN_FILENO);
		dup2(get_data(NULL)->backup_out, STDOUT_FILENO);
		close(get_data(NULL)->backup_out);
		close(get_data(NULL)->backup_in);
		get_data(NULL)->backup_out = -1;
		get_data(NULL)->backup_in = -1;
		return (OK);
	}
	return (NO);
}

static void	wait_for_finish(pid_t last_command)
{
	int			status;
	pid_t		w;

	while (1)
	{
		w = wait(&status);
		if (w < 0)
		{
			if (errno == ECHILD)
				break ;
			else
				perror("wait");
		}
		else
		{
			if (w == last_command)
			{
				if (WIFEXITED(status))
					get_data(NULL)->last_ex_code = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
					get_data(NULL)->last_ex_code = 128 + WTERMSIG(status);
			}
		}
	}
}

	//g_foreground = 1;
	//g_foreground = 0;
t_err	execute(t_cmd_table *table)
{
	t_cmd_table	*current;
	pid_t		last;

	if (all_pipes_init(table) == FAIL)
		return (ERROR);
	get_data(NULL)->heredoc_active = 1;
	if (heredoc(table) == FAIL)
	{
		get_data(NULL)->heredoc_active = 0;
		return (ERROR);
	}
	get_data(NULL)->heredoc_active = 0;
	if (single_builtin_line(table) != NO)
		return (OK);
	current = table;
	last = -1;
	while (current)
	{
		execute_single(current, table);
		last = current->pid;
		current = current->next;
	}
	close_unused_pipes(table);
	wait_for_finish(last);
	return (OK);
}

/*
!!IMPORTANT
!Check if this is true - but its also possible to revert back to originals and
! close current
!anyways. CEHCK THIS!
!When Markus exits from builtin. He must if (data->fd != -1) -> CLOSE!!!
!chatGDP say close dup() even if no used!


!redo return value of: return (OK);//redo this return value! (first function)
!redo return value of single_builtin_line in last func
!maybe add fprintf for error return to main.c?
*/

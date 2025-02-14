/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 14:37:39 by dbogovic          #+#    #+#             */
/*   Updated: 2025/02/01 17:06:24 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	try_execve(t_cmd_table *table)
{
	char	*path;
	t_err	reason;

	path = get_path(table->cmd);
	if (!path)
		return (get_data(NULL)->last_ex_code);

	set_signals_default(); // MAK CHANGE
	//set_signals(3);
	//signal(SIGQUIT, sigquit_handler);  // MAK CHANGE

	execve(path, table->args, get_data(NULL)->mini_envp);
	if (ft_strncmp(path, table->cmd, ft_strlen(path)) == 0)
	{
		reason = is_proper_exe(path, 0);
		print_path_err(reason, table->cmd);
	}
	free(path);
	return (get_data(NULL)->last_ex_code);
}

static void	child(t_cmd_table *table, t_cmd_table *head)
{
	int	exit_code;

	pipe_redir(table);
	close_unused_pipes(head);
	if (redir(table->redir_data, table->redir_data) == FAIL)
	{
		free_data();
		free_table(head);
		exit(1);
	}
	if (!table->cmd)
	{
		exit_code = get_data(NULL)->last_ex_code;
		free_table(head);
		free_data();
		exit(exit_code);
	}
	if (is_builtin(table->cmd))
		exit_code = try_builtin(table);
	else
		exit_code = try_execve(table);
	free_data();
	free_table(head);
	exit(exit_code);
}

int	execute_single(t_cmd_table *table, t_cmd_table *head)
{
	pid_t		child_id;

	child_id = fork();
	if (child_id < 0)
	{
		perror("fork");
		return (-1);
	}
	if (child_id == 0)
	{
		child(table, head);
	}
	else
	{
		table->pid = child_id;
	}
	return (0);
}

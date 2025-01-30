/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 14:37:39 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/29 18:57:56 by dbogovic         ###   ########.fr       */
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
	execve(path, table->args, get_data(NULL)->mini_envp);
	if (ft_strncmp(path, table->cmd, ft_strlen(path)) == 0)
	{
		reason = is_proper_exe(path, 0);
		print_path_err(reason, table->cmd);
	}
	free(path);
	return (get_data(NULL)->last_ex_code);
}

static void	child(t_cmd_table *table, t_data *data, t_cmd_table *head)
{
	int	exit_code;

	pipe_redir(table);
	close_unused_pipes(head);
	if (redir(table->redir_data, table->redir_data) == FAIL)
	{
		free_data();
		(void)data;
		free_table(head);
		exit(1);
	}
	if (!table->cmd)
		exit(get_data(NULL)->last_ex_code);
	if (is_builtin(table->cmd))
		exit(try_builtin(table));
	exit_code = try_execve(table);
	free_data();
	(void)data;
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
		child(table, get_data(NULL), head);
	}
	else
	{
		table->pid = child_id;
	}
	return (0);
}

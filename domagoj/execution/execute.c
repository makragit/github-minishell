/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:12:41 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/19 15:04:37 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_err	try_execve(t_cmd_table *table)
{
	char	**paths;
	size_t	c;

	if (!table->cmd)
		return (FAIL);
	paths = get_path(table->cmd);
	c = 0;
	if (paths)
	{
		while (paths[c])
		{
			execve(paths[c], table->args, NULL);
			c++;
		}
	}
	printf("%s: command not found\n", table->cmd);
	ft_free_array(paths);
	return (FAIL);
}

void	child(t_cmd_table *table, t_data *data, t_cmd_table *head)
{
	connect_pipes(table);
	if (redir(table) == FAIL)
	{
		free(data);
		free_table(head);
		exit(1);
	}
	if (try_execve(table) == FAIL)
	{
		free(data);
		free_table(head);
		exit(1);
	}
}

int	execute_single(t_cmd_table *table, t_cmd_table *head)
{
	pid_t		child_id;
	int			status;
	t_data		*data;

	data = get_data(NULL);
	child_id = fork();
	if (child_id == 0)
	{
		child(table, data, head);
	}
	else if (child_id > 0)
	{
		if (waitpid(child_id, &status, 0) == -1)
			return (-1);
		if (WIFEXITED(status))
			data->last_ex_code = WEXITSTATUS(status);
	}
	else
	{
		status = -1;
		perror("fork");
	}
	close_pipes(table);
	return (status);
}

void	in_out_backup(t_cmd_table *table, t_err mode)
{
	if (mode == CAPTURE)
	{
		table->stdin_backup = dup(STDERR_FILENO);
		table->stdin_backup = dup(STDOUT_FILENO);
	}
	else if (mode == RESTORE)
	{
		dup2(table->stdin_backup, STDOUT_FILENO);
		dup2(table->stdin_backup, STDIN_FILENO);
	}
}

t_err	execute(t_cmd_table *table)
{
	t_cmd_table	*current;
	int			status;

	current = table;
	in_out_backup(table, CAPTURE);
	if (pipe_setup(table) == FAIL)
	{
		printf("fatal error! pipe setup failed!\n");
		return (ERROR);
	}
	if (heredoc(table) == FAIL)
		return (ERROR);
	while (current)
	{
		if (is_builtin(current->cmd))
		{
			redir(current);
			in_out_backup(table, RESTORE);
			connect_pipes(current);
			try_builtin(current);
			in_out_backup(table, RESTORE);
			current = current->next;
			continue ;
		}
		if (execute_single(current, table) == -1)
			return (ERROR);
		current = current->next;
	}
	while (wait(&status) > 0)
	{
	}
	in_out_backup(table, RESTORE);
	return (OK);
}
/*

musnt go inside func if there is !table
change exit code for when fork fails
add markus's builtin
*/






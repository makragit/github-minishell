/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:12:41 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/07 21:11:27 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../../minishell.h"


t_err	try_execve(t_cmd_table *table)
{
	char	**paths;
	size_t	c;
	t_data	*data;

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
	data = get_data(NULL);
	free(data);
	ft_free_array(paths);
	return (FAIL);
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
		connect_pipes(table);
		if (redir(table) == FAIL)
		{
			free(data);
			free_table(head);
			exit(1);
		}
		if (try_execve(table) == FAIL)
		{
			free_table(head);
			exit(1);
		}
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
		status = 11; //change to errno thing
		perror("fork");
	}
	close_pipes(table);
	return (status);
}

t_err	execute(t_cmd_table *table)
{
	t_cmd_table	*current;
	int			status;

	//if (try_builtin(table) != -1)
//	{
		//change last ex code;
		//return fail on the whole thing
//	}
	current = table;
	if (!table)
	{
		printf("No table error\n");
		return (ERROR);
	}
	table->stdin_backup = dup(STDERR_FILENO);
	table->stdin_backup = dup(STDOUT_FILENO);
	if (pipe_setup(table) == FAIL)
	{
		printf("fatal error! pipe setup failed!\n");
		return (ERROR);
	}
	if (heredoc(table) == FAIL)
		return (ERROR);
	while (current)
	{
		if (execute_single(current, table) == 11)
			return (ERROR);
		current = current->next;
	}
	while (wait(&status) > 0);
	dup2(table->stdin_backup, STDOUT_FILENO);
	dup2(table->stdin_backup, STDIN_FILENO);
	return (OK);
}

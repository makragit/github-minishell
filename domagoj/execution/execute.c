/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:12:41 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/22 16:41:04 by dbogovic         ###   ########.fr       */
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
			execve(paths[c], table->args, get_data(NULL)->mini_envp); // MAK CHANGE: NULL to mini_envp
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
		// MAK CHANGE: free_data() fixes leaks
		// free(data);
		free_data();
		(void)data;

		free_table(head);
		exit(1);
	}
	if (try_execve(table) == FAIL)
	{
		// MAK CHANGE: free_data() fixes leaks
		// free(data);
		free_data();
		(void)data;
		free_table(head);
		exit(1);
	}
}

int	execute_single(t_cmd_table *table, t_cmd_table *head)
{
	pid_t		child_id;
	int			status;
	t_data		*data;

	if (!table->cmd)
		return (0);
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

t_err	execute(t_cmd_table *table)
{
	t_cmd_table	*current;
	int			status;
	int			fd_out;
	int			fd_in;

	current = table;
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
			fd_out = dup(STDOUT_FILENO);
			fd_in = dup(STDIN_FILENO);
			redir(current);
			connect_pipes(current);
			// try_builtin(current);//! this shoudlnt exit on its own
			get_data(NULL)->last_ex_code = try_builtin(current);//! this shoudlnt exit on its own
			dup2(fd_out, STDOUT_FILENO);
			dup2(fd_in, STDIN_FILENO);
			close(fd_out);
			close(fd_in);
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
	return (OK);
}



/*

musnt go inside func if there is !table
change exit code for when fork fails
add markus's builtin
*/

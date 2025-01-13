/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:25:39 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/10 16:42:01 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	connect_pipes(t_cmd_table *table)
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

static t_err	open_fd(char *filename, t_token_type mode)
{
	int	fd;

	fd = -1;
	if (mode == HEREDOC)
	{
		fd = open (filename, O_RDONLY);
		if (unlink(filename) == -1)
		{
			perror("unlink");
			free(filename);
			close(fd);
			return (FAIL);
		}
	}
	if (mode == APPEND)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (mode == INPUT)
		fd = open(filename, O_RDONLY);
	else if (mode == OUTPUT)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (FAIL);
	if (mode == APPEND || mode == OUTPUT)
		dup2(fd, STDOUT_FILENO);
	else if (mode == INPUT || mode == HEREDOC)
		dup2(fd, STDIN_FILENO);
	close(fd);
	return (OK);
}

t_err	redir(t_cmd_table *table)
{
	t_redir_data	*head;
	t_err			result;

	result = OK;
	if (!table->redir_data)
		return (OK);
	head = table->redir_data;
	while (table->redir_data)
	{
		if (table->redir_data->heredoc_file_name)
			result = open_fd(table->redir_data->heredoc_file_name, HEREDOC);
		if (result == FAIL)
			break ;
		if (table->redir_data->input)
			result = open_fd(table->redir_data->input, INPUT);
		if (result == FAIL)
			break ;
		if (table->redir_data->append)
			result = open_fd(table->redir_data->append, APPEND);
		if (result == FAIL)
			break ;
		if (table->redir_data->output)
			result = open_fd(table->redir_data->output, OUTPUT);
		if (result == FAIL)
			break ;
		table->redir_data = table->redir_data->next;
	}
	table->redir_data = head;
	return (result);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:25:39 by dbogovic          #+#    #+#             */
/*   Updated: 2025/02/15 12:57:31 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	open_heredoc(char *filename)
{
	int	fd;

	fd = open (filename, O_RDONLY);
	if (fd == -1)
	{
		perror("");
		return (-1);
	}
	if (unlink(filename) == -1)
	{
		perror("unlink");
		return (-1);
	}
	return (fd);
}

static t_err	open_fd(char *filename, t_token_type mode)
{
	int	fd;

	if (mode == HEREDOC)
	{
		fd = open_heredoc(filename);
	}
	if (mode == APPEND)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (mode == INPUT)
		fd = open(filename, O_RDONLY);
	else if (mode == OUTPUT)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		print_path_err(PERRORR, filename);
		return (FAIL);
	}
	if (mode == APPEND || mode == OUTPUT)
		dup2(fd, STDOUT_FILENO);
	else if (mode == INPUT || mode == HEREDOC)
		dup2(fd, STDIN_FILENO);
	close(fd);
	return (OK);
}

t_err	redir(t_redir_data *lst)
{
	t_err			result;

	result = OK;
	while (lst)
	{
		if (lst->heredoc_file_name)
			result = open_fd(lst->heredoc_file_name, HEREDOC);
		if (result == FAIL)
			break ;
		if (lst->input)
			result = open_fd(lst->input, INPUT);
		if (result == FAIL)
			break ;
		if (lst->append)
			result = open_fd(lst->append, APPEND);
		if (result == FAIL)
			break ;
		if (lst->output)
			result = open_fd(lst->output, OUTPUT);
		lst = lst->next;
	}
	if (result == FAIL)
		get_data(NULL)->last_ex_code = 1;
	return (result);
}

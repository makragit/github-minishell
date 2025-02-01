/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_program.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 16:44:59 by dbogovic          #+#    #+#             */
/*   Updated: 2025/02/01 15:33:36 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

t_err	cmp_line(const char *str1, const char *str2)
{
	int	return_value;

	if (!str2 || !str1)
		return (ERROR);
	if (str2[0] == '\'' || str2[0] == '\"')
		return_value = ft_strncmp(str1, str2 + 1, ft_strlen(str2 + 1) - 1);
	else
		return_value = ft_strncmp(str1, str2, ft_strlen(str2));
	if (return_value == 0)
		return (EQUAL);
	return (FAIL);
}

char	*expand_line(char *line, char *delim, int is_quoted)
{
	if (!line || !delim)
		return (NULL);
	if (is_quoted == 1)
		return (line);
	if (expand_env(&line))
		return (NULL);
	return (line);
}

int	fetch_herdoc(char *heredoc_delim, int file_descriptor, int is_quoted)
{
	char	*line;
	char	*processed_line;

	while (1)
	{
		line = readline("heredoc:");
		if (!line)
		{
			write(2, "Readline error!\n", 16);
			return (1);
		}
		if (cmp_line(line, heredoc_delim) == EQUAL)
			break ;
		processed_line = expand_line(line, heredoc_delim, is_quoted);
		if (!processed_line)
			return (ERROR);
		ft_append(processed_line, file_descriptor);
		free(processed_line);
	}
	return (0);
}

int	heredoc(t_cmd_table *table)
{
	t_redir_data	*current;
	char			*filename;
	int				fd;

	filename = NULL;
	while (table)
	{
		current = table->redir_data;
		while (current)
		{
			fd = -1;
			if (current->hdoc_delim)
			{
				fd = ft_create_file(&filename);
				if (fetch_herdoc(current->hdoc_delim, fd, current->is_quoted))
					return (1);
				table->redir_data->heredoc_file_name = filename;
				close(fd);
			}
			current = current->next;
		}
		table = table->next;
	}
	return (0);
}
/*
!fix- will this func reshuffle redir_data?
!fix - newline doesnt print out?
!safe cpy
int	heredoc(t_cmd_table *table)
{
	t_redir_data	*current;
	char			*filename;
	int				fd;

	fd = -1;
	filename = NULL;
	while (table)
	{
		current = table->redir_data;
		while (current)
		{
			if (current->hdoc_delim)
			{
				if (fd == -1)
					fd = ft_create_file(&filename);
				if (fetch_herdoc(current->hdoc_delim, fd, current->is_quoted))
					return (1);
				table->redir_data->heredoc_file_name = filename;
			}
			if (fd != -1)
				close(fd);
			fd = -1;
			current = current->next;
		}
		table = table->next;
	}
	return (0);
}
*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_program.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 16:44:59 by dbogovic          #+#    #+#             */
/*   Updated: 2025/02/02 14:58:45 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

t_err	cmp_line(char *str1, const char *str2)
{
	int	return_value;

	if (!str2 || !str1)
		return (ERROR);
	if (str2[0] == '\'' || str2[0] == '\"')
		return_value = ft_strncmp(str1, str2 + 1, ft_strlen(str2 + 1) - 1);
	else
		return_value = ft_strncmp(str1, str2, ft_strlen(str2));
	if (return_value == 0)
	{
		free(str1);
		return (EQUAL);
	}
	return (FAIL);
}

char	*expand_line(char *line, char *delim, int is_quoted)
{
	char	*tmp;

	if (!line || !delim)
		return (NULL);
	if (is_quoted == 1)
		return (line);
	if (expand_env(&line))
		return (NULL);
	tmp = ft_strjoin(line, "\n");
	free(line);
	line = tmp;
	return (line);
}

int	fetch_herdoc(char *heredoc_delim, int file_descriptor, int is_quoted)
{
	char	*line;
	char	*processed_line;

	while (1)
	{
		line = read_line();
		if (!line)
		{
			write(2, "minishell: here-doc delimited by EOF\n", 37);
			close(file_descriptor);
			return (1);
		}
		if (cmp_line(line, heredoc_delim) == EQUAL)
			break ;
		processed_line = expand_line(line, heredoc_delim, is_quoted);
		if (!processed_line)
		{
			close(file_descriptor);
			return (1);
		}
		ft_append(processed_line, file_descriptor);
		free(processed_line);
	}
	close(file_descriptor);
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
			if (current->hdoc_delim)
			{
				fd = ft_create_file(&filename);
				if (fetch_herdoc(current->hdoc_delim, fd, current->is_quoted))
				{
					unlink(filename);
					free(filename); // MAK CHANGE
					return (FAIL);
				}
				table->redir_data->heredoc_file_name = filename;
			}
			current = current->next;
		}
		table = table->next;
	}
	return (0);
}

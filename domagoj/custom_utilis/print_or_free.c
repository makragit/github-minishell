/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_or_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:39:10 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/05 19:09:39 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_array(char **array)
{
	size_t	i;

	if (array == NULL)
		return ;
	i = 0;
	while (array[i] != NULL)
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}

void	free_table(t_cmd_table *table)
{
	t_cmd_table		*tmp;
	t_redir_data	*redir_tmp;

	free_lst(table->tokens);
	table->tokens = NULL;
	if (table)
		close_pipes(table);
	while (table)
	{
		if (table->heredoc_fd != -1)
			close(table->heredoc_fd);
		while (table->redir_data)
		{
			redir_tmp = table->redir_data;
			table->redir_data = table->redir_data->next;
			free(redir_tmp);
			redir_tmp = NULL;
		}
		tmp = table->next;
		free(table->args);
		table->args = NULL;
		free(table);
		table = NULL;
		table = tmp;
	}
}

void	free_lst(t_token *lst)
{
	t_token	*tmp;

	if (!lst)
		return ;
	while (lst)
	{
		tmp = lst;
		lst = lst->next;
		free(tmp->value);
		tmp->value = NULL;
		free(tmp);
		tmp = NULL;
	}
}

static void	args_print(char **args)
{
	size_t	g;

	if (!args)
		return ;
	printf("{");
	g = 0;
	while (args[g])
	{
		printf("%s, ", args[g]);
		g++;
	}
	printf("NULL}\n");
}

void	cmd_print(t_cmd_table *table)
{
	t_redir_data	*redirs;

	while (table)
	{
		printf("---TABLE INDEX: %i!---\n", table->index);
		printf("command |%s|\n", table->cmd);
		if (table->args)
			args_print(table->args);
		if (table->redir_data)
		{
			printf("HELLO WORLD\n");
			redirs = table->redir_data;
			while (redirs)
			{
				printf("append_file: %s, ", redirs->append);
				printf("input_file: %s, ", redirs->input);
				printf("output_file: %s, ", redirs->output);
				printf("heredoc_delim: %s, ", redirs->heredoc_delimiter);
				printf("\n");
				redirs = redirs->next;
			}
		}
		table = table->next;
	}
}

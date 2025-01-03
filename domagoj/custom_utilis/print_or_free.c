/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_or_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 15:39:10 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/03 18:26:52 by mkrausho         ###   ########.fr       */
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

	while (table)
	{
		tmp = table->next;
		free(table->input_redir);
		free(table->output_redir);
		free(table->append_redir);
		free(table->heredoc_delim);
		table->input_redir = NULL;
		table->output_redir = NULL;
		table->append_redir = NULL;
		table->heredoc_delim = NULL;
		free_array(table->args);
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
		free(tmp);
		tmp = NULL;
	}
}

void	fully_free(t_token *lst)
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
	while (table)
	{
		printf("---TABLE INDEX: %i!---\n", table->index);
		printf("command |%s|\n", table->cmd);
		if (table->input_redir)
			printf("%s: input\n", table->input_redir);
		if (table->output_redir)
			printf("%s: output\n", table->output_redir);
		if (table->append_redir)
			printf("%s: append\n", table->append_redir);
		if (table->heredoc_delim)
			printf("%s: heredoc\n", table->heredoc_delim);
		if (table->args)
			args_print(table->args);
		table = table->next;
	}
}
